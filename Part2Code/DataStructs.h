#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const int MAX_NODES = 100;
const int MAX_PLAYERS = 64;
const int MAX_TEAM_SIZE = 5;

// ===============================
// DOUBLY LINKED LIST NODE
// ===============================

template<typename T>
struct DoublyNode {
    T data;
    DoublyNode* next;
    DoublyNode* prev;
    
    DoublyNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

// ===============================
// QUEUE IMPLEMENTATION
// ===============================

template<typename T>
class Queue {
private:
    DoublyNode<T>* queueFront;
    DoublyNode<T>* queueRear;
    int count;

public:
    Queue() {
        queueFront = nullptr;
        queueRear = nullptr;
        count = 0;
    }
    
    ~Queue() {
        while (!isEmpty()) {
            dequeue();
        }
    }
    
    void enqueue(T elem) {
        DoublyNode<T>* newNode = new DoublyNode<T>(elem);
        
        if (isEmpty()) {
            queueFront = newNode;
            queueRear = newNode;
        } else {
            queueRear->next = newNode;
            newNode->prev = queueRear;
            queueRear = newNode;
        }
        count++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty. Cannot delete element.");
        }
        
        DoublyNode<T>* tempNode = queueFront;
        T tempInfo = queueFront->data;
        
        queueFront = queueFront->next;
        
        if (queueFront == nullptr) {
            queueRear = nullptr;
        } else {
            queueFront->prev = nullptr;
        }
        
        delete tempNode;
        count--;
        
        return tempInfo;
    }
    
    bool isEmpty() {
        return (queueFront == nullptr);
    }
    
    int size() {
        return count;
    }
    
    T front() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return queueFront->data;
    }
    
    T rear() {
        if (isEmpty()) {
            throw runtime_error("Queue is empty");
        }
        return queueRear->data;
    }
    
    DoublyNode<T>* getHead() {
        return queueFront;
    }
};

// ===============================
// STACK IMPLEMENTATION
// ===============================

template<typename T>
class Stack {
private:
    DoublyNode<T>* head;
    int count;

public:
    Stack() { 
        this->head = nullptr; 
        this->count = 0;
    }
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    bool isEmpty() {
        return head == nullptr;
    }
    
    void push(T new_data) {
        DoublyNode<T>* new_node = new DoublyNode<T>(new_data);
        if (!new_node) {
            cout << "\nStack Overflow";
            return;
        }
        new_node->next = head;
        if (head != nullptr) {
            head->prev = new_node;
        }
        head = new_node;
        count++;
    }
    
    T pop() {
        if (this->isEmpty()) {
            throw runtime_error("Stack Underflow");
        } else {
            DoublyNode<T>* temp = head;
            T data = head->data;
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
            delete temp;
            count--;
            return data;
        }
    }
    
    T peek() {
        if (!isEmpty())
            return head->data;
        else {
            throw runtime_error("Stack is empty");
        }
    }
    
    int size() {
        return count;
    }
};

// ===============================
// PRIORITY QUEUE IMPLEMENTATION
// ===============================

template<typename T>
class PriorityQueue {
private:
    struct Node {
        T data;
        int priority;
        Node* next;
        
        Node(const T& d, int p) : data(d), priority(p), next(nullptr) {}
    };
    
    Node* head;
    int count;
    
public:
    PriorityQueue() : head(nullptr), count(0) {}
    
    ~PriorityQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void enqueue(const T& item, int priority) {
        Node* newNode = new Node(item, priority);
        
        if (!head || priority > head->priority) {
            newNode->next = head;
            head = newNode;
            count++;
            return;
        }
        
        Node* current = head;
        while (current->next && current->next->priority >= priority) {
            current = current->next;
        }
        
        newNode->next = current->next;
        current->next = newNode;
        count++;
    }
    
    T dequeue() {
        if (!head) {
            throw runtime_error("Priority Queue is empty!");
        }
        
        T result = head->data;
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
        
        return result;
    }
    
    T peek() {
        if (!head) {
            throw runtime_error("Priority Queue is empty!");
        }
        return head->data;
    }
    
    bool isEmpty() { 
        return head == nullptr; 
    }
    
    int size() {
        return count;
    }
};

#endif // DATASTRUCTURES_H