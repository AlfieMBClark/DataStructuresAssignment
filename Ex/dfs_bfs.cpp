#include <iostream>
using namespace std;

const int MAX_NODES = 100;

// Node structure for adjacency list
struct AdjNode {
    int vertex;
    AdjNode* next;
};

// Stack Data Structure
struct Stack {
    int data[MAX_NODES];
    int top = -1;

    void push(int value) {
        if (top < MAX_NODES - 1) {
            data[++top] = value;
        }
    }

    int pop() {
        if (top >= 0) {
            return data[top--];
        }
        return -1;
    }

    bool isEmpty() {
        return top == -1;
    }
};

// Queue Data Structure
struct Queue {
    int data[MAX_NODES];
    int front = 0;
    int rear = 0;

    void enqueue(int value) {
        if (rear < MAX_NODES) {
            data[rear++] = value;
        }
    }

    int dequeue() {
        if (front < rear) {
            return data[front++];
        }
        return -1;
    }

    bool isEmpty() {
        return front == rear;
    }
};

// Graph structure
struct Graph {
    int numVertices;
    AdjNode* adjLists[MAX_NODES];

    // Constructor to initialize the graph
    Graph(int vertices) {
        numVertices = vertices;
        for (int i = 0; i < vertices; i++) {
            adjLists[i] = nullptr;
        }
    }

    // Add an edge (undirected graph)
    void addEdge(int src, int dest) {
        // Add edge from src to dest
        AdjNode* newNode = new AdjNode{dest, adjLists[src]};
        adjLists[src] = newNode;
        
        // Add edge from dest to src (since it's undirected)
        newNode = new AdjNode{src, adjLists[dest]};
        adjLists[dest] = newNode;
    }

    // Print the graph
    void printGraph() {
        for (int i = 0; i < numVertices; i++) {
            cout << "Vertex " << i << ":";
            AdjNode* temp = adjLists[i];
            while (temp) {
                cout << " -> " << temp->vertex;
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // DFS Traversal
    void DFS(int start) {
        bool visited[MAX_NODES] = {false};
        Stack stack;
        stack.push(start);

        cout << "DFS starting from vertex " << start << ": ";
        while (!stack.isEmpty()) {
            int current = stack.pop();
            if (!visited[current]) {
                cout << current << " ";
                visited[current] = true;

                // Push all unvisited adjacent vertices to the stack
                AdjNode* temp = adjLists[current];
                while (temp) {
                    if (!visited[temp->vertex]) {
                        stack.push(temp->vertex);
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
    }

    // BFS Traversal
    void BFS(int start) {
        bool visited[MAX_NODES] = {false};
        Queue queue;
        queue.enqueue(start);
        visited[start] = true;

        cout << "BFS starting from vertex " << start << ": ";
        while (!queue.isEmpty()) {
            int current = queue.dequeue();
            cout << current << " ";

            // Enqueue all unvisited adjacent vertices
            AdjNode* temp = adjLists[current];
            while (temp) {
                if (!visited[temp->vertex]) {
                    queue.enqueue(temp->vertex);
                    visited[temp->vertex] = true;
                }
                temp = temp->next;
            }
        }
        cout << endl;
    }

    // Destructor to free dynamically allocated memory
    ~Graph() {
        for (int i = 0; i < numVertices; i++) {
            AdjNode* temp = adjLists[i];
            while (temp) {
                AdjNode* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

int main() {
    Graph g(6);

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    g.printGraph();
    
    g.DFS(0);
    g.BFS(0);

    return 0;
}
