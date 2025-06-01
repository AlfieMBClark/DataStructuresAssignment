#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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

// ===== PRIORITY QUEUE NODE =====
template<typename T>
class PriorityQueueNode {
public:
    T data;
    int priority;
    PriorityQueueNode* next;

    PriorityQueueNode() : priority(0), next(nullptr) {}
    
    PriorityQueueNode(T item, int prio) : data(item), priority(prio), next(nullptr) {}
};

// ===== PRIORITY QUEUE =====
template<typename T>
class PriorityQueueHeap {
private:
    PriorityQueueNode<T>* front;
    int size;

public:
    PriorityQueueHeap() : front(nullptr), size(0) {}

    ~PriorityQueueHeap() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(T item, int priority) {
        PriorityQueueNode<T>* newNode = new PriorityQueueNode<T>(item, priority);
        
        if (front == nullptr || priority > front->priority) {
            newNode->next = front;
            front = newNode;
        } else {
            PriorityQueueNode<T>* current = front;
            while (current->next != nullptr && current->next->priority >= priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        size++;
    }

    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Priority Queue is empty - cannot dequeue");
        }
        
        PriorityQueueNode<T>* temp = front;
        T data = front->data;
        front = front->next;
        delete temp;
        size--;
        return data;
    }

    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Priority Queue is empty - cannot peek");
        }
        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return size;
    }

    void display() const {
        if (isEmpty()) {
            cout << "Priority Queue is empty" << endl;
            return;
        }
        
        PriorityQueueNode<T>* current = front;
        cout << "Priority Queue Contents (High to Low Priority):" << endl;
        int position = 1;
        while (current != nullptr) {
            cout << position << ". Priority " << current->priority << ": ";
            current->data.displaySpectator();
            current = current->next;
            position++;
        }
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};

// ===== CIRCULAR QUEUE =====
template<typename T>
class CircularQueue {
private:
    T* queue;
    int front;
    int rear;
    int capacity;
    int size;

public:
    CircularQueue(int cap) : capacity(cap), front(-1), rear(-1), size(0) {
        queue = new T[capacity];
    }

    ~CircularQueue() {
        delete[] queue;
    }

    bool isEmpty() const {
        return size == 0;
    }

    bool isFull() const {
        return size == capacity;
    }

    // Add element to rear
    void enqueue(T item) {
        if (isFull()) {
            throw runtime_error("Circular Queue is full - cannot enqueue");
        }
        
        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % capacity;
        }
        
        queue[rear] = item;
        size++;
    }

    // Remove and return element from front
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Circular Queue is empty - cannot dequeue");
        }
        
        T item = queue[front];
        if (size == 1) {
            front = rear = -1;
        } else {
            front = (front + 1) % capacity;
        }
        size--;
        return item;
    }

    // View front element without removing
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Circular Queue is empty - cannot peek");
        }
        return queue[front];
    }

    int getSize() const {
        return size;
    }

    int getCapacity() const {
        return capacity;
    }

    // Display all elements in queue order
    void display() const {
        if (isEmpty()) {
            cout << "Circular Queue is empty" << endl;
            return;
        }
        
        cout << "Circular Queue Contents:" << endl;
        int current = front;
        for (int i = 0; i < size; i++) {
            cout << (i + 1) << ". ";
            queue[current].displaySlot();
            current = (current + 1) % capacity;
        }
    }

    // Get element at specific position (for rotation management)
    T getAt(int index) const {
        if (isEmpty() || index < 0 || index >= size) {
            throw runtime_error("Invalid index in circular queue");
        }
        
        int position = (front + index) % capacity;
        return queue[position];
    }

    // Rotate to next element (dequeue and enqueue)
    void rotate() {
        if (!isEmpty()) {
            T item = dequeue();
            enqueue(item);
        }
    }

    // Clear all elements
    void clear() {
        front = rear = -1;
        size = 0;
    }
};

// ===== SPECTATOR QUEUE NODE =====
template<typename T>
class SpectatorQueueNode {
public:
    T data;
    SpectatorQueueNode* next;

    SpectatorQueueNode() : next(nullptr) {}
    SpectatorQueueNode(T item) : data(item), next(nullptr) {}
};

// ===== SPECTATOR OVERFLOW QUEUE =====
template<typename T>
class SpectatorQueue {
private:
    SpectatorQueueNode<T>* front;
    SpectatorQueueNode<T>* rear;
    int size;

public:
    SpectatorQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~SpectatorQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Add element to rear (FIFO)
    void enqueue(T item) {
        SpectatorQueueNode<T>* newNode = new SpectatorQueueNode<T>(item);
        
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    // Remove and return element from front
    T dequeue() {
        if (isEmpty()) {
            throw runtime_error("Spectator Queue is empty - cannot dequeue");
        }
        
        SpectatorQueueNode<T>* temp = front;
        T data = front->data;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        delete temp;
        size--;
        return data;
    }

    // View front element without removing
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("Spectator Queue is empty - cannot peek");
        }
        return front->data;
    }

    bool isEmpty() const {
        return front == nullptr;
    }

    int getSize() const {
        return size;
    }

    // Display all elements in FIFO order
    void display() const {
        if (isEmpty()) {
            cout << "Spectator Overflow Queue is empty" << endl;
            return;
        }
        
        SpectatorQueueNode<T>* current = front;
        cout << "Spectator Overflow Queue Contents:" << endl;
        int position = 1;
        while (current != nullptr) {
            cout << position << ". ";
            current->data.displaySpectator();
            current = current->next;
            position++;
        }
    }

    // Clear all elements
    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }
};
#endif 