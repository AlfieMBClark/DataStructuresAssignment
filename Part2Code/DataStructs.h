#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const int MAX_NODES = 100;
const int MAX_PLAYERS = 64;
const int MAX_TEAM_SIZE = 5;

template<typename T>
struct DoublyNode {
    T data;
    DoublyNode* next;
    DoublyNode* prev;
    
    DoublyNode(T value) : data(value), next(nullptr), prev(nullptr) {}
};

// ===== FIXED QUEUE WITH COPY SUPPORT =====
template<typename T>
class Queue {
private:
    DoublyNode<T>* queueFront;
    DoublyNode<T>* queueRear;
    int count;

public:
    Queue() : queueFront(nullptr), queueRear(nullptr), count(0) {}
    
    // Copy constructor (REQUIRED FOR ASSIGNMENT COMPLIANCE)
    Queue(const Queue& other) : queueFront(nullptr), queueRear(nullptr), count(0) {
        DoublyNode<T>* current = other.queueFront;
        while (current != nullptr) {
            enqueue(current->data);
            current = current->next;
        }
    }
    
    // Assignment operator (REQUIRED FOR ASSIGNMENT COMPLIANCE)
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            while (!isEmpty()) {
                dequeue();
            }
            
            DoublyNode<T>* current = other.queueFront;
            while (current != nullptr) {
                enqueue(current->data);
                current = current->next;
            }
        }
        return *this;
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
    
    bool isEmpty() const { return (queueFront == nullptr); }
    int size() const { return count; }
    T front() const {
        if (isEmpty()) throw runtime_error("Queue is empty");
        return queueFront->data;
    }
    T rear() const {
        if (isEmpty()) throw runtime_error("Queue is empty");
        return queueRear->data;
    }
    DoublyNode<T>* getHead() const { return queueFront; }
};

// ===== FIXED STACK WITH COPY SUPPORT =====
template<typename T>
class Stack {
private:
    DoublyNode<T>* head;
    int count;

public:
    Stack() : head(nullptr), count(0) {}
    
    // Copy constructor (REQUIRED)
    Stack(const Stack& other) : head(nullptr), count(0) {
        Stack<T> temp;
        DoublyNode<T>* current = other.head;
        while (current != nullptr) {
            temp.push(current->data);
            current = current->next;
        }
        
        while (!temp.isEmpty()) {
            push(temp.pop());
        }
    }
    
    // Assignment operator (REQUIRED)
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            while (!isEmpty()) {
                pop();
            }
            
            Stack<T> temp;
            DoublyNode<T>* current = other.head;
            while (current != nullptr) {
                temp.push(current->data);
                current = current->next;
            }
            
            while (!temp.isEmpty()) {
                push(temp.pop());
            }
        }
        return *this;
    }
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    bool isEmpty() const { return head == nullptr; }
    
    void push(T new_data) {
        DoublyNode<T>* new_node = new DoublyNode<T>(new_data);
        new_node->next = head;
        if (head != nullptr) {
            head->prev = new_node;
        }
        head = new_node;
        count++;
    }
    
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack Underflow");
        }
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
    
    T peek() const {
        if (!isEmpty()) return head->data;
        throw runtime_error("Stack is empty");
    }
    
    int size() const { return count; }
};

// ===== PRIORITY QUEUE REMAINS THE SAME =====
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
    
    T peek() const {
        if (!head) {
            throw runtime_error("Priority Queue is empty!");
        }
        return head->data;
    }
    
    bool isEmpty() const { return head == nullptr; }
    int size() const { return count; }
};

#endif 