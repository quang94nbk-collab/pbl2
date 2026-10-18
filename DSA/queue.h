#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>

using namespace std;

template <typename T>
struct QueueNode {
    T value;
    QueueNode<T> *next;
    QueueNode(const T& val = T());
};

template <typename T>
QueueNode<T>::QueueNode(const T& val) : value(val), next(nullptr) {}

template <typename T>
class Queue {
private:
    QueueNode<T> *frontNode;
    QueueNode<T> *rearNode;

public:
    Queue();
    ~Queue();

    bool empty() const;
    void push(const T& x);
    bool pop(T &x);
    bool front(T &x) const;
    void print() const;
    void clear();
};

template <typename T>
Queue<T>::Queue() : frontNode(nullptr), rearNode(nullptr) {}

template <typename T>
Queue<T>::~Queue() {
    clear();
}

template <typename T>
bool Queue<T>::empty() const {
    return frontNode == nullptr;
}

template <typename T>
void Queue<T>::push(const T& x) {
    QueueNode<T> *p = new QueueNode<T>(x);
    if (empty()) {
        frontNode = rearNode = p;
    } else {
        rearNode->next = p;
        rearNode = p;
    }
}

template <typename T>
bool Queue<T>::pop(T &x) {
    if (empty()) return false;
    QueueNode<T> *p = frontNode;
    x = p->value;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        rearNode = nullptr;
    }
    delete p;
    return true;
}

template <typename T>
bool Queue<T>::front(T &x) const {
    if (empty()) return false;
    x = frontNode->value;
    return true;
}

template <typename T>
void Queue<T>::print() const {
    QueueNode<T> *p = frontNode;
    while (p != nullptr) {
        cout << p->value << " ";
        p = p->next;
    }
    cout << "\n";
}

template <typename T>
void Queue<T>::clear() {
    T dummy;
    while (pop(dummy));
}

#endif
