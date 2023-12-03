import requests
import os
from datetime import datetime, timedelta

"""
    This script's purpose is to download a file off the internet that
    consists of all 1,000,000 movie IDs in The Movie Database.
    This file is updated every day, hence the datetime library.
    The output is a timestamped .gz file located in ../data/raw_data wherein each line is a JSON object.
"""

# Function to download the previous day's movie IDs file
def download_movie_ids():
    # Calculate yesterday's date
    # Can't use today's date because the website updates on PST, so after 8:00 PM PST (midnight EST),
    # the datetime library will try to find a URL that won't exist for another four hours.
    yesterday = datetime.utcnow() - timedelta(days=1)
    date_str = yesterday.strftime("%m_%d_%Y")
    url = f"http://files.tmdb.org/p/exports/movie_ids_{date_str}.json.gz"
    response = requests.get(url, stream=True)

    # Ensure the correct directories exist
    parent_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), os.pardir)
    dir_path = os.path.join(parent_dir, "data", "raw_data")
    os.makedirs(dir_path, exist_ok=True)

    file_path = os.path.join(dir_path, f"movie_ids_{date_str}.json.gz")

    with open(file_path, "wb") as file:
        for chunk in response.iter_content(chunk_size=8192):
            file.write(chunk)

    print(f"Movie IDs file downloaded to {file_path}")

download_movie_ids()