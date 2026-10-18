#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <iostream>

using namespace std;

template <typename T>
struct NodeDLL
{
    T data;
    NodeDLL<T> *next;
    NodeDLL<T> *pre;

    NodeDLL(T x);
};

template <typename T>
NodeDLL<T>::NodeDLL(T x)
{
    data = x;
    next = NULL;
    pre = NULL;
}

template <typename T>
class DoubleLinkedList
{
private:
    NodeDLL<T> *head;

    bool checkNode(NodeDLL<T> *p) const;

public:
    DoubleLinkedList();
    ~DoubleLinkedList();
    DoubleLinkedList(const DoubleLinkedList&) = delete;
    DoubleLinkedList& operator=(const DoubleLinkedList&) = delete;

    bool empty() const;

    void pushFront(T x);
    void pushBack(T x);

    NodeDLL<T> *find(T x) const;

    void insertAfter(NodeDLL<T> *p, T x);
    void insertBefore(NodeDLL<T> *p, T x);

    void eraseHead();
    void eraseTail();
    void eraseNode(NodeDLL<T> *p);

    void eraseValue(T x);
    void eraseAllValue(T x);

    void replaceFirst(T x, T y);
    void replaceAll(T x, T y);

    void print() const;
    void clear();

    NodeDLL<T> *getHead() const;
};

template <typename T>
DoubleLinkedList<T>::DoubleLinkedList()
{
    head = NULL;
}

template <typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
    clear();
}

template <typename T>
bool DoubleLinkedList<T>::empty() const
{
    return head == NULL;
}

template <typename T>
bool DoubleLinkedList<T>::checkNode(NodeDLL<T> *p) const
{
    return p != NULL;
}

template <typename T>
void DoubleLinkedList<T>::pushFront(T x)
{
    NodeDLL<T> *p = new NodeDLL<T>(x);

    if (head == NULL)
    {
        head = p;
        return;
    }

    p->next = head;
    head->pre = p;
    head = p;
}

template <typename T>
void DoubleLinkedList<T>::pushBack(T x)
{
    NodeDLL<T> *p = new NodeDLL<T>(x);

    if (head == NULL)
    {
        head = p;
        return;
    }

    NodeDLL<T> *q = head;

    while (q->next != NULL)
    {
        q = q->next;
    }

    q->next = p;
    p->pre = q;
}

template <typename T>
NodeDLL<T> *DoubleLinkedList<T>::find(T x) const
{
    NodeDLL<T> *p = head;

    while (p != NULL)
    {
        if (p->data == x)
        {
            return p;
        }

        p = p->next;
    }

    return NULL;
}

template <typename T>
void DoubleLinkedList<T>::insertAfter(NodeDLL<T> *p, T x)
{
    if (!checkNode(p))
    {
        return;
    }

    NodeDLL<T> *q = new NodeDLL<T>(x);

    q->next = p->next;
    q->pre = p;

    if (p->next != NULL)
    {
        p->next->pre = q;
    }

    p->next = q;
}

template <typename T>
void DoubleLinkedList<T>::insertBefore(NodeDLL<T> *p, T x)
{
    if (!checkNode(p))
    {
        return;
    }

    if (p == head)
    {
        pushFront(x);
        return;
    }

    NodeDLL<T> *q = new NodeDLL<T>(x);

    q->pre = p->pre;
    q->next = p;

    p->pre->next = q;
    p->pre = q;
}

template <typename T>
void DoubleLinkedList<T>::eraseHead()
{
    if (head == NULL)
    {
        return;
    }

    NodeDLL<T> *p = head;

    head = head->next;

    if (head != NULL)
    {
        head->pre = NULL;
    }

    delete p;
}

template <typename T>
void DoubleLinkedList<T>::eraseTail()
{
    if (head == NULL)
    {
        return;
    }

    NodeDLL<T> *p = head;

    while (p->next != NULL)
    {
        p = p->next;
    }

    if (p->pre != NULL)
    {
        p->pre->next = NULL;
    }
    else
    {
        head = NULL;
    }

    delete p;
}

template <typename T>
void DoubleLinkedList<T>::eraseNode(NodeDLL<T> *p)
{
    if (!checkNode(p))
    {
        return;
    }

    if (p == head)
    {
        eraseHead();
        return;
    }

    if (p->pre != NULL)
    {
        p->pre->next = p->next;
    }

    if (p->next != NULL)
    {
        p->next->pre = p->pre;
    }

    delete p;
}

template <typename T>
void DoubleLinkedList<T>::eraseValue(T x)
{
    NodeDLL<T> *p = find(x);

    if (p != NULL)
    {
        eraseNode(p);
    }
}

template <typename T>
void DoubleLinkedList<T>::eraseAllValue(T x)
{
    NodeDLL<T> *p = head;

    while (p != NULL)
    {
        NodeDLL<T> *q = p->next;

        if (p->data == x)
        {
            eraseNode(p);
        }

        p = q;
    }
}

template <typename T>
void DoubleLinkedList<T>::replaceFirst(T x, T y)
{
    NodeDLL<T> *p = find(x);

    if (p != NULL)
    {
        p->data = y;
    }
}

template <typename T>
void DoubleLinkedList<T>::replaceAll(T x, T y)
{
    NodeDLL<T> *p = head;

    while (p != NULL)
    {
        if (p->data == x)
        {
            p->data = y;
        }

        p = p->next;
    }
}

template <typename T>
void DoubleLinkedList<T>::print() const
{
    NodeDLL<T> *p = head;

    while (p != NULL)
    {
        cout << p->data << " ";
        p = p->next;
    }

    cout << endl;
}

template <typename T>
void DoubleLinkedList<T>::clear()
{
    while (head != NULL)
    {
        eraseHead();
    }
}

template <typename T>
NodeDLL<T> *DoubleLinkedList<T>::getHead() const
{
    return head;
}

#endif
