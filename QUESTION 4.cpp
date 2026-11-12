
#include <iostream>
#include <string>
#include <algorithm>
#include <climits>

using namespace std;

const int N = 20;
const int M = 30;
const int INF = 999999;

string districts[N] = {
    "Karonga", "Rumphi", "Mzimba", "Nkhata Bay",
    "Kasungu", "Nkhotakota", "Salima", "Lilongwe",
    "Dedza", "Ntcheu", "Balaka", "Mangochi",
    "Machinga", "Zomba", "Blantyre", "Mwanza",
    "Thyolo", "Mulanje", "Chikwawa", "Nsanje"
};

// Road connections: starting district, ending district, distance
int roads[M][3] = {
    {0,1,150}, {1,2,70}, {2,3,50}, {2,4,230},
    {3,5,250}, {4,5,135}, {4,7,130}, {5,6,120},
    {6,7,100}, {6,11,160}, {6,8,120}, {7,8,85},
    {8,9,60}, {9,10,80}, {9,15,115}, {10,11,90},
    {10,12,50}, {10,14,140}, {11,12,70}, {12,13,50},
    {13,14,65}, {13,17,105}, {14,16,40}, {14,15,65},
    {14,17,80}, {14,18,105}, {15,18,80}, {16,17,70},
    {16,18,95}, {18,19,90}
};

int graph[N][N];

// Build the road network
void buildGraph() {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            graph[i][j] = 0;
        }
    }

    for (i = 0; i < M; i++) {
        int u = roads[i][0];
        int v = roads[i][1];
        int w = roads[i][2];

        graph[u][v] = w;
        graph[v][u] = w;
    }
}

// Display all districts
void displayDistricts() {
    int i;

    cout << "\n----- MALAWI DISTRICTS -----\n";

    for (i = 0; i < N; i++) {
        cout << i + 1 << ". " << districts[i] << endl;
    }
}

// Dijkstra's shortest path algorithm
void dijkstra() {
    int source, destination;
    int distance[N];
    int parent[N];
    bool visited[N];

    displayDistricts();

    cout << "\nEnter starting district number: ";
    cin >> source;

    cout << "Enter destination district number: ";
    cin >> destination;

    if (source < 1 || source > N ||
        destination < 1 || destination > N) {
        cout << "\nInvalid district number!\n";
        return;
    }

    source--;
    destination--;

    int i, j;

    for (i = 0; i < N; i++) {
        distance[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    distance[source] = 0;

    for (i = 0; i < N; i++) {
        int u = -1;
        int minimum = INF;

        // Find the nearest unvisited district
        for (j = 0; j < N; j++) {
            if (!visited[j] && distance[j] < minimum) {
                minimum = distance[j];
                u = j;
            }
        }

        if (u == -1)
            break;

        visited[u] = true;

        // Update distances to neighbouring districts
        for (j = 0; j < N; j++) {
            if (graph[u][j] > 0 && !visited[j]) {
                if (distance[u] + graph[u][j] < distance[j]) {
                    distance[j] = distance[u] + graph[u][j];
                    parent[j] = u;
                }
            }
        }
    }

    if (distance[destination] == INF) {
        cout << "\nNo route found!\n";
        return;
    }

    // Construct the shortest route
    int path[N];
    int count = 0;
    int current = destination;

    while (current != -1) {
        path[count] = current;
        count++;
        current = parent[current];
    }

    cout << "\n----- SHORTEST ROUTE ----\n";

    for (i = count - 1; i >= 0; i--) {
        cout << districts[path[i]];

        if (i > 0)
            cout << " -> ";
    }

    cout << "\nTotal distance: "
         << distance[destination] << " km\n";
}

// Find the root of a set
int findRoot(int parent[], int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }

    return x;
}

// Kruskal's Minimum Spanning Tree algorithm
void kruskal() {
    int parent[N];
    int sortedRoads[M][3];
    int i, j;

    // Initialise parent array
    for (i = 0; i < N; i++) {
        parent[i] = i;
    }

    // Copy roads
    for (i = 0; i < M; i++) {
        for (j = 0; j < 3; j++) {
            sortedRoads[i][j] = roads[i][j];
        }
    }

    // Sort roads by distance using bubble sort
    for (i = 0; i < M - 1; i++) {
        for (j = 0; j < M - i - 1; j++) {
            if (sortedRoads[j][2] > sortedRoads[j + 1][2]) {
                for (int k = 0; k < 3; k++) {
                    int temp = sortedRoads[j][k];
                    sortedRoads[j][k] = sortedRoads[j + 1][k];
                    sortedRoads[j + 1][k] = temp;
                }
            }
        }
    }

    int total = 0;
    int count = 0;

    cout << "\n----- KRUSKAL MINIMUM SPANNING TREE -----\n";

    for (i = 0; i < M; i++) {
        int u = sortedRoads[i][0];
        int v = sortedRoads[i][1];
        int w = sortedRoads[i][2];

        int rootU = findRoot(parent, u);
        int rootV = findRoot(parent, v);

        // Select road if it does not form a cycle
        if (rootU != rootV) {
            parent[rootU] = rootV;

            cout << districts[u] << " - "
                 << districts[v] << " : "
                 << w << " km\n";

            total += w;
            count++;

            if (count == N - 1)
                break;
        }
    }

    cout << "\nNumber of selected roads: " << count;
    cout << "\nTotal MST distance: " << total << " km\n";

    if (count != N - 1) {
        cout << "The graph is disconnected.\n";
    }
}

// Main program
int main() {
    int choice;

    buildGraph();

    do {
        cout << "\n----------------------------------\n";
        cout << "    MALAWI DISTRICT ROAD NETWORK\n";
        cout << "-----------------------------------\n";

        cout << "1. Display Districts\n";
        cout << "2. Find Shortest Route (Dijkstra)\n";
        cout << "3. Minimum Spanning Tree (Kruskal)\n";
        cout << "4. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayDistricts();
                break;

            case 2:
                dijkstra();
                break;

            case 3:
                kruskal();
                break;

            case 4:
                cout << "\nProgram ended successfully.\n";
                break;

            default:
                cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}

