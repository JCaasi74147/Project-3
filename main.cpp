#include <iostream>
#include <vector>
#include <unordered_map>
#include "graph.cpp"
#include "dijkstra.cpp"
#include "bfs.cpp"
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {
    // Check if the correct number of arguments are passed
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <start_actor_name> <target_actor_name>" << endl;
        return 0;
    }

    string startActorName = argv[1];
    string targetActorName = argv[2];

    Graph movieGraph;
    movieGraph.buildGraphFromCSV("actor-movie-data.csv");
    unordered_map<int, string> actorNames = movieGraph.getActorNames();

    // Find the actor IDs corresponding to the entered names
    int startActorId = -1, targetActorId = -1;
    for (const auto& pair : actorNames) {
        if (pair.second == startActorName) {
            startActorId = pair.first;
        }
        if (pair.second == targetActorName) {
            targetActorId = pair.first;
        }
    }

    if (startActorId == -1 || targetActorId == -1) {
        cout << "Actor name(s) not found in the dataset." << endl;
        return 0;
    }

    // You can choose a default algorithm or add another argument to specify it
    string algorithm = argv[3];  // "dijkstra" or "bfs"

    auto start = chrono::high_resolution_clock::now();vector<int> path;
    if (algorithm == "dijkstra") {
        path = dijkstra(movieGraph, startActorId, targetActorId);
    } else if (algorithm == "bfs") {
        path = bfs(movieGraph, startActorId, targetActorId);
    } else {
        cerr << "Invalid algorithm choice." << endl;
        return 0;
    }
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::ofstream file("times.txt");
    if (file.is_open()) {
        file << "Time taken by function: "
             << duration.count() << " microseconds" << std::endl;
        file.close();
    } else {
        std::cerr << "Unable to open file" << std::endl;
    }

    if (path.empty()) {
        cout << "No path exists between " << startActorName << " and " << targetActorName << endl;
        return 0;
    }

    // Output the shortest path
    int n = 0;
    for (auto it = path.begin(); it != path.end() - 1; it++) {
        if (n % 2 == 0) {
            cout << actorNames.at(*it) << "<";
        } else {
            cout << movieGraph.getMovieNames().at(*it) << "<";
        }
        n++;
    }
    cout << actorNames.at(*(path.end() - 1)) << endl;


    return 0;
}