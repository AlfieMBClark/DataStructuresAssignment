#ifndef TOURNAMENT_GRAPH_H
#define TOURNAMENT_GRAPH_H

#include "DataStructs.h"
#include <iostream>
#include <string>

using namespace std;

// ===============================
// GRAPH STRUCTURE FOR TOURNAMENT BRACKET
// ===============================

struct AdjNode {
    int matchVertex;
    string matchType;
    AdjNode* next;
    
    AdjNode(int vertex, string type) : matchVertex(vertex), matchType(type), next(nullptr) {}
};

struct TournamentGraph {
    int numMatches;
    AdjNode* adjLists[MAX_NODES];
    string matchNames[MAX_NODES];
    
    TournamentGraph(int matches = 0) {
        numMatches = matches;
        for (int i = 0; i < MAX_NODES; i++) {
            adjLists[i] = nullptr;
            matchNames[i] = "";
        }
    }
    
    void addMatchConnection(int srcMatch, int destMatch, string connectionType) {
        if (srcMatch < MAX_NODES && destMatch < MAX_NODES) {
            AdjNode* newNode = new AdjNode(destMatch, connectionType);
            newNode->next = adjLists[srcMatch];
            adjLists[srcMatch] = newNode;
        }
    }
    
    void setMatchName(int matchIndex, string matchID) {
        if (matchIndex < MAX_NODES) {
            matchNames[matchIndex] = matchID;
        }
    }
    
    void printTournamentBracket() {
        cout << "\n=== TOURNAMENT BRACKET STRUCTURE ===\n";
        for (int i = 0; i < numMatches; i++) {
            cout << "Match " << i << " (" << matchNames[i] << "):";
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                cout << " -> Match " << temp->matchVertex 
                     << "(" << temp->matchType << ")";
                temp = temp->next;
            }
            cout << endl;
        }
    }
    
    ~TournamentGraph() {
        for (int i = 0; i < MAX_NODES; i++) {
            AdjNode* temp = adjLists[i];
            while (temp != nullptr) {
                AdjNode* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
        }
    }
};

// ===============================
// DFS AND BFS FOR TOURNAMENT TRAVERSAL
// ===============================

class TournamentTraversal {
private:
    TournamentGraph* graph;
    
    struct SimpleStack {
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
    
    struct SimpleQueue {
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
    
public:
    TournamentTraversal(TournamentGraph* g) : graph(g) {}
    
    void DFS(int start) {
        bool visited[MAX_NODES] = {false};
        SimpleStack stack;
        stack.push(start);
        
        cout << "\nDFS Tournament Bracket Traversal starting from match " << start << ": ";
        while (!stack.isEmpty()) {
            int current = stack.pop();
            if (!visited[current] && current < graph->numMatches) {
                cout << graph->matchNames[current] << " ";
                visited[current] = true;
                
                AdjNode* temp = graph->adjLists[current];
                while (temp) {
                    if (!visited[temp->matchVertex]) {
                        stack.push(temp->matchVertex);
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
    }
    
    void BFS(int start) {
        bool visited[MAX_NODES] = {false};
        SimpleQueue queue;
        queue.enqueue(start);
        visited[start] = true;
        
        cout << "BFS Tournament Bracket Traversal starting from match " << start << ": ";
        while (!queue.isEmpty()) {
            int current = queue.dequeue();
            if (current < graph->numMatches) {
                cout << graph->matchNames[current] << " ";
                
                AdjNode* temp = graph->adjLists[current];
                while (temp) {
                    if (!visited[temp->matchVertex]) {
                        queue.enqueue(temp->matchVertex);
                        visited[temp->matchVertex] = true;
                    }
                    temp = temp->next;
                }
            }
        }
        cout << endl;
    }
};

#endif // TOURNAMENT_GRAPH_H