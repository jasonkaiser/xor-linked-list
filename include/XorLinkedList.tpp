//
// Created by aldin on 18/03/2025.
//

#ifndef XORLINKEDLIST_TPP
#define XORLINKEDLIST_TPP

#include <cstdint>
#include <iterator>

/**
 * These methods are already implemented for you.
 */
template <typename Data>
Node<Data>* ptr_xor(Node<Data> *p, Node<Data> *q) {
    return reinterpret_cast<Node<Data>*>(reinterpret_cast<uintptr_t>(p) ^ reinterpret_cast<uintptr_t>(q));
}


template<typename Data>
Node<Data> *XorLinkedList<Data>::get_head() const {
    return head;
}

/**
 * Your implementations below this comment.
 */

template<typename Data>
XorLinkedList<Data>::~XorLinkedList() {
    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;

    while (curr) {
        Node<Data>* next = ptr_xor(prev, curr->both);
        delete curr;
        prev = curr;
        curr = next;
    }
}

template<typename Data>
XorLinkedList<Data>::XorLinkedList(const XorLinkedList<Data> &src) {
    head = nullptr;
    if (!src.head) return;

    Node<Data>* prevSrc = nullptr;
    Node<Data>* currSrc = src.head;
    Node<Data>* prevNew = nullptr;

    while (currSrc) {
        Node<Data>* newNode = new Node<Data>{currSrc->data, prevNew};

        if (!head) {
            head = newNode;
        } else {
            prevNew->both = ptr_xor(prevNew->both, newNode);
        }

        Node<Data>* nextSrc = ptr_xor(prevSrc, currSrc->both);
        prevSrc = currSrc;
        currSrc = nextSrc;
        prevNew = newNode;
    }
}

template<typename Data>
XorLinkedList<Data> &XorLinkedList<Data>::operator=(const XorLinkedList<Data> &src) {
    if (this == &src) return *this;

    this->~XorLinkedList();

    head = nullptr;
    if (!src.head) return *this;

    Node<Data>* prevSrc = nullptr;
    Node<Data>* currSrc = src.head;
    Node<Data>* prevNew = nullptr;

    while (currSrc) {
        Node<Data>* newNode = new Node<Data>{currSrc->data, prevNew};

        if (!head) {
            head = newNode;
        } else {
            prevNew->both = ptr_xor(prevNew->both, newNode);
        }

        Node<Data>* nextSrc = ptr_xor(prevSrc, currSrc->both);
        prevSrc = currSrc;
        currSrc = nextSrc;
        prevNew = newNode;
    }

    return *this;
}

template<typename Data>
XorLinkedList<Data>::XorLinkedList(XorLinkedList<Data> &&src) noexcept {
    head = src.head;
    src.head = nullptr;
}

template<typename Data>
XorLinkedList<Data> &XorLinkedList<Data>::operator=(XorLinkedList<Data> &&src) noexcept {
    if (this == &src) return *this;

    this->~XorLinkedList();

    head = src.head;
    src.head = nullptr;

    return *this;
}

template<typename Data>
XorLinkedList<Data>::XorLinkedList(std::initializer_list<Data> list) {
    head = nullptr;
    for (const Data& item : list) {
        add_to_back(item);
    }
}

template<typename Data>
void XorLinkedList<Data>::add_to_front(const Data &data) {
    Node<Data>* newNode = new Node<Data>{data, head};
    if (head) {
        head->both = ptr_xor(newNode, head->both);
    }
    head = newNode;
}

template<typename Data>
void XorLinkedList<Data>::add_to_back(const Data &data) {
    Node<Data>* newNode = new Node<Data>{data, nullptr};

    if (!head) {
        head = newNode;
        return;
    }

    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;
    Node<Data>* next = ptr_xor(prev, curr->both);

    while (next) {
        prev = curr;
        curr = next;
        next = ptr_xor(prev, curr->both);
    }

    newNode->both = curr;
    curr->both = ptr_xor(prev, newNode);
}

template<typename Data>
void XorLinkedList<Data>::remove_from_front() {
    if (!head) return;

    Node<Data>* next = head->both;
    if (next) {
        next->both = ptr_xor(head, next->both);
    }
    delete head;
    head = next;
}

template<typename Data>
void XorLinkedList<Data>::remove_from_back() {
    if (!head) {
        throw std::out_of_range("Cant remove from empty list");
    }

    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;
    Node<Data>* next = ptr_xor(prev, curr->both);

    while (next) {
        prev = curr;
        curr = next;
        next = ptr_xor(prev, curr->both);
    }

    if (prev) {
        Node<Data>* prevPrev = ptr_xor(prev->both, curr);
        prev->both = prevPrev;
    }
    else {
        head = nullptr;
    }

    delete curr;
}

template<typename Data>
Data &XorLinkedList<Data>::get(int index) {
    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;
    int i = 0;

    while (curr && i < index) {
        Node<Data>* next = ptr_xor(prev, curr->both);
        prev = curr;
        curr = next;
        i++;
    }

    if (!curr) throw std::out_of_range("Index out of bounds");
    return curr->data;
}

template<typename Data>
int XorLinkedList<Data>::count() const {
    int count = 0;
    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;

    while (curr) {
        count++;
        Node<Data>* next = ptr_xor(prev, curr->both);
        prev = curr;
        curr = next;
    }

    return count;
}

template<typename Data>
void XorLinkedList<Data>::reverse() {
    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;
    Node<Data>* next = nullptr;

    while (curr) {
        next = ptr_xor(prev, curr->both);
        curr->both = ptr_xor(next, prev);
        prev = curr;
        curr = next;
    }

    head = prev;
}

template <typename Data>
class XorLinkedList<Data>::Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Data;
    using difference_type = std::ptrdiff_t;
    using pointer = Data*;
    using reference = Data&;
    
private:
    Node<Data>* current;
    Node<Data>* prev;

public:
    explicit Iterator(Node<Data>* current) : current(current), prev(nullptr) {}

    Data& operator*() {
        return current->data;
    }

    Iterator& operator++() {
        if (!current) return *this;
        Node<Data>* next = ptr_xor(prev, current->both);
        prev = current;
        current = next;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const Iterator& other) const {
        return current == other.current;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }
};

template<typename Data>
typename XorLinkedList<Data>::Iterator XorLinkedList<Data>::begin() {
    return Iterator(head);
}

template<typename Data>
typename XorLinkedList<Data>::Iterator XorLinkedList<Data>::end() {
    return Iterator(nullptr);
}

template<typename Data>
const Data& XorLinkedList<Data>::operator[](int index) const {
    Node<Data>* prev = nullptr;
    Node<Data>* curr = head;
    for (int i = 0; i < index && curr; ++i) {
        Node<Data>* next = ptr_xor(prev, curr->both);
        prev = curr;
        curr = next;
    }
    if (!curr) throw std::out_of_range("Index out of bounds");
    return curr->data;
}

template<typename Data>
Data& XorLinkedList<Data>::operator[](const int index) {
    return const_cast<Data&>(static_cast<const XorLinkedList&>(*this)[index]);
}

#endif //XORLINKEDLIST_TPP