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
    bool contains(const T& x) const;
    void print() const;
    void clear();
    int size() const;

    /**
     * @brief Xoa phan tu bat ky khoi hang doi (Queue).
     * @param x Gia tri can xoa khoi hang doi.
     * @return true neu tim thay va da xoa thanh cong, false neu khong tim thay.
     */
    bool remove(const T& x);

    template <typename Container>
    void copyTo(Container& c) const;
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
bool Queue<T>::contains(const T& x) const {
    QueueNode<T> *p = frontNode;
    while (p != nullptr) {
        if (p->value == x) return true;
        p = p->next;
    }
    return false;
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

template <typename T>
int Queue<T>::size() const {
    int cnt = 0;
    QueueNode<T> *p = frontNode;
    while (p != nullptr) {
        cnt++;
        p = p->next;
    }
    return cnt;
}

// -------------------------------------------------------------
// Chuc nang: Xoa mot phan tu cu the ra khoi hang doi lien ket don
// Cap nhat lai frontNode, rearNode neu phan tu bi xoa o dau/cuoi
// -------------------------------------------------------------
template <typename T>
bool Queue<T>::remove(const T& x) {
    bool found = false;
    QueueNode<T> *cur = frontNode;
    QueueNode<T> *prev = nullptr;
    while (cur != nullptr) {
        if (cur->value == x) {
            found = true;
            QueueNode<T> *toDelete = cur;
            if (prev == nullptr) {
                frontNode = cur->next;
                cur = frontNode;
            } else {
                prev->next = cur->next;
                cur = prev->next;
            }
            if (toDelete == rearNode) {
                rearNode = prev;
            }
            delete toDelete;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
    if (frontNode == nullptr) rearNode = nullptr;
    return found;
}

template <typename T>
template <typename Container>
void Queue<T>::copyTo(Container& c) const {
    QueueNode<T> *p = frontNode;
    while (p != nullptr) {
        c.pushBack(p->value);
        p = p->next;
    }
}

#endif
