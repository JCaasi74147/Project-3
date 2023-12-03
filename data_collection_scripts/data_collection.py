'''
    ## Data collection for local database ##
    The script below performs the following:
    1) initializes a connection pool for the sql database.
    2) defines two helper functions that leverage the API to gather specific data about a certain movieID.
    3) defines a function to insert the data into the sql database.
    4) processes the IDs and iterates over every movie ID contained within the file in data/raw_data,
       calling the insert function.
'''

import os
import json
import gzip
import requests
import psycopg2
from datetime import datetime, timedelta
from psycopg2 import pool

api_key = "removed :)"
api_url = "https://api.themoviedb.org/3"

# Initialize a connection pool for postgreSQL
conn_pool = psycopg2.pool.SimpleConnectionPool(
    1, 20,
    dbname='mydatabase',
    user='postgres',
    password='hibiscus',
    host='localhost',
    port='5432',
    options='-c client_encoding=UTF8'
)

# Function to directly update the movie names of a given list of movie IDs.
# Created to mend the mistake of originally using movies' "original_name" attribute
# which were often invalid for UTF-8.
def update_wrong_names():
    with open("wrongnames.txt", "r") as file:
        for line in file:
            wrong_movie_id = int(line.strip())  # Assuming each line contains only the movie ID
            correct_movie_data = get_movie_data(wrong_movie_id)
            correct_title = correct_movie_data.get("title", None)

            if correct_title:
                connection = conn_pool.getconn()
                cursor = connection.cursor()

                try:
                    cursor.execute(
                        """
                        UPDATE movies
                        SET original_title = %s
                        WHERE movie_id = %s;
                        """,
                        (correct_title, wrong_movie_id)
                    )
                    connection.commit()
                except Exception as e:
                    print(f"An error occurred while updating movie ID {wrong_movie_id}: {e}")
                    connection.rollback()
                finally:
                    cursor.close()
                    conn_pool.putconn(connection)

def get_movie_data(movie_id):
    url = f"{api_url}/movie/{movie_id}?language=en-US"
    headers = {
        "accept": "application/json",
        "Authorization": f"Bearer {api_key}"
    }
    response = requests.get(url, headers=headers)
    return response.json()

def get_credits_data(movie_id):
    url = f"{api_url}/movie/{movie_id}/credits?language=en-US"
    headers = {
        "accept": "application/json",
        "Authorization": f"Bearer {api_key}"
    }
    response = requests.get(url, headers=headers)
    return response.json()

def insert_data(movie_data, credits_data):
    connection = conn_pool.getconn()
    cursor = connection.cursor()

    try:
        # Insert movie data
        cursor.execute(
            """
            INSERT INTO movies (movie_id, original_title, popularity, vote_average)
            VALUES (%s, %s, %s, %s);
            """,
            (movie_data["id"], movie_data["title"], movie_data["popularity"], movie_data["vote_average"])
        )

        # Insert actors and relationships
        for cast_member in credits_data['cast']:
            if cast_member['known_for_department'] == 'Acting':
                cursor.execute(
                    "INSERT INTO actors (actor_id, name) VALUES (%s, %s) ON CONFLICT (actor_id) DO NOTHING",
                    (cast_member['id'], cast_member['name'])
                )
                cursor.execute(
                    "INSERT INTO movie_actor (movie_id, actor_id) VALUES (%s, %s) ON CONFLICT (movie_id, actor_id) DO NOTHING",
                    (movie_data['id'], cast_member['id'])
                )

        connection.commit()
    except Exception as e:
        print(f"An error occurred: {e}")
        connection.rollback()
    finally:
        cursor.close()
        conn_pool.putconn(connection)

def process_movie_ids(file_path):
    movie_ids = []
    with gzip.open(file_path, "rt", encoding="utf-8") as file:
        for line in file:
            movie_data = json.loads(line)
            movie_ids.append(movie_data['id'])
    return movie_ids

if __name__ == "__main__":
    # Get yesterday's date for filepath use
    yesterday = datetime.utcnow() - timedelta(days=1)
    date_str = yesterday.strftime("%m_%d_%Y")
    parent_dir = os.path.abspath(
        os.path.join(os.path.dirname(__file__), os.pardir))  # Get the parent directory of the script
    file_path = os.path.join(parent_dir, "data", "raw_data", f"movie_ids_{date_str}.json.gz")

    # Get the list of movie IDs
    movie_ids = process_movie_ids(file_path)

    ### update_wrong_names()

    start_fetching = False
    # Iterate through the movie IDs, fetch data, and insert it into the database
    for movie_id in movie_ids:
        if movie_id > 1195480:
            # used to save time for whenever the script is terminated for whatever reason.
            start_fetching = True

        if start_fetching:
            movie_data = get_movie_data(movie_id)
            if movie_data.get('vote_count', 0) <= 1:
                # insufficient vote count. we don't want irrelevant movies.
                # this was added after movie_id ~150,000, so entries before might include irrelevant data.
                continue
            if 'success' in movie_data and not movie_data['success']:
                # error-handling implemented after an unfortunate error forced script to terminate at 3:00AM after 4 hours
                print(f"Could not fetch data for movie ID: {movie_data.get('id', 'Unknown')}")
                continue
            credits_data = get_credits_data(movie_id)
            insert_data(movie_data, credits_data)