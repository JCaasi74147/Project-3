import json
import gzip
import os
from datetime import datetime, timedelta

'''
    The 'process_IDs' script will read each line as a json object to populate and return a simple list of all movie IDs.
    This is step 2. See 'downloads_ids' for step 1.
'''

# Function to process the movie IDs file
def process_movie_ids(file_path):
    movie_ids = []

    with gzip.open(file_path, "rt", encoding="utf-8") as file:
        for line in file:
            movie_data = json.loads(line)
            movie_ids.append(movie_data['id'])

    return movie_ids


# get yesterday's date for file path
yesterday = datetime.utcnow() - timedelta(days=1)
date_str = yesterday.strftime("%m_%d_%Y")
parent_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir))  # Get the parent directory of the script
file_path = os.path.join(parent_dir, "data", "raw_data", f"movie_ids_{date_str}.json.gz")  # Using the correct parent directory

# call the function with the path to the downloaded file
movie_ids = process_movie_ids(file_path)