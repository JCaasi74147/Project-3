from flask import Flask, request, jsonify, render_template
import subprocess
import threading
import os
import logging

app = Flask(__name__)

logging.basicConfig(level=logging.DEBUG)

def run_cpp_program(start_actor, end_actor, algorithm):

    command = ['./main', start_actor, end_actor, algorithm]
    logging.info(f"Executing command: {command}")

    # Run the command and capture the output
    try:
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, check=True)
        logging.info("C++ program output: " + result.stdout)
        logging.error("C++ program error (if any): " + result.stderr)
    except subprocess.CalledProcessError as e:
        logging.error("Error running C++ program: " + str(e))

    # Write the output to a file
    with open('final_result.txt', 'w') as file:
        file.write(result.stdout)

@app.route('/')
def index():
    return render_template('web.html')

@app.route('/start-process', methods=['POST'])
def start_process():
    reset_files()
    data = request.json
    start_actor = data['startActor']
    end_actor = data['endActor']
    algorithm = data['algorithm']  # This will be either 'dijkstra' or 'bfs'

    # start the cpp program with the chosen algorithm **in a separate thread
    threading.Thread(target=run_cpp_program, args=(start_actor, end_actor, algorithm)).start()
    logging.info("C++ program thread started with " + algorithm + " algorithm.")

    return jsonify({"message": "Process started with " + algorithm + " algorithm"}), 202

@app.route('/check-progress')
def check_progress():
    progress = read_progress_from_file()
    logging.info(f"Current progress: {progress}")  # Log the current progress
    return jsonify({"progress": progress})

@app.route('/get-final-result')
def get_final_result():
    final_result = read_final_result()
    return jsonify({'result': final_result})

def read_final_result():
    result_file_path = 'final_result.txt'

    if os.path.exists(result_file_path):
        with open(result_file_path, 'r') as file:
            return file.read().strip()
    else:
        return "Result not available yet."

def read_progress_from_file():
    progress_file_path = 'progress.txt'

    try:
        if os.path.exists(progress_file_path):
            with open(progress_file_path, 'r') as file:
                for last_line in file:
                    pass
                progress = last_line.strip()
                logging.info(f"Read progress: {progress}")
                return int(float(progress))  # Convert string to float, then to int
        else:
            logging.info("Progress file not found.")
            return 0
    except Exception as e:
        logging.error(f"Error reading progress file: {e}")
        return 0


def reset_files():
    with open('final_result.txt', 'w') as file:
        file.truncate(0)  # Clear the contents of the file

    with open('progress.txt', 'w') as file:
        file.truncate(0)

@app.route('/clear-files', methods=['GET'])
def clear_files():
    reset_files()
    return jsonify({"message": "Results cleared successfully"})

if __name__ == '__main__':
    app.run(debug=True)