#include <iostream>
using namespace std;

const int MAX_NODES = 100;

// Node structure for adjacency list
struct AdjNode {
    int vertex;
    AdjNode* next;
};

// Graph structure
struct Graph {
    int numVertices;
    AdjNode* adjLists[MAX_NODES];  // Array of adjacency lists

    // Constructor
    Graph(int vertices) {
        numVertices = vertices;
        for (int i = 0; i < vertices; i++) {
            adjLists[i] = nullptr;
        }
    }

    // Add edge (directed)
    void addEdge(int src, int dest) {
        AdjNode* newNode = new AdjNode{dest, adjLists[src]};
        adjLists[src] = newNode;
    }

    // Print the graph
    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << ":";
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                cout << " -> " << temp->vertex;
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // Destructor to free memory
    ~Graph() {
        for (int i = 0; i < numVertices; i++) {
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                AdjNode* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

int main() {
    Graph g(5);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
    g.addEdge(4, 0);

    g.printGraph();

    return 0;
}
