#include <iostream>
using namespace std;

const int MAX_NODES = 100;  // Maximum number of nodes

// Node structure for adjacency list
struct AdjNode {
    int vertex;
    AdjNode* next;
};

// Graph structure
struct Graph {
    int numVertices;
    AdjNode* adjLists[MAX_NODES];  // Array of pointers to AdjNode

    // Constructor
    Graph(int vertices) {
        numVertices = vertices;
        for (int i = 0; i < vertices; i++) {
            adjLists[i] = nullptr;
        }
    }

    // Add edge (undirected graph)
    void addEdge(int src, int dest) {
        // Add edge from src to dest
        AdjNode* newNode = new AdjNode{dest, adjLists[src]};
        adjLists[src] = newNode;

        // Add edge from dest to src
        newNode = new AdjNode{src, adjLists[dest]};
        adjLists[dest] = newNode;
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
    g.addEdge(0, 4);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 3);
    g.addEdge(3, 4);

    g.printGraph();

    return 0;
}
