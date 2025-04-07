//
// Created by aldin on 18/03/2025.
//

#ifndef XORLINKEDLIST_H
#define XORLINKEDLIST_H
#include "Node.h"
#include <initializer_list>

template<typename Data>
class XorLinkedList {
private:
    Node<Data>* head{};
    int size{0};
public:
    // constructors
    XorLinkedList() = default;
    XorLinkedList(std::initializer_list<Data> list);
    // copy semantics
    XorLinkedList(const XorLinkedList<Data>& src);
    XorLinkedList<Data>& operator=(const XorLinkedList<Data>& src);
    // move semantics
    XorLinkedList(XorLinkedList<Data>&& src) noexcept;
    XorLinkedList<Data>& operator=(XorLinkedList<Data>&& src) noexcept;
    // destructor
    ~XorLinkedList();

    void add_to_front(const Data& data);
    void add_to_back(const Data& data);
    void remove_from_front();
    void remove_from_back();
    Data& get(int index);
    int count() const;
    void reverse();
    // iterator
    class Iterator;
    Iterator begin();
    Iterator end();
    // overloading []
    const Data& operator[](int index) const;
    Data& operator[](int index);

    // Already implemented for your; do not modify.
    Node<Data>* get_head() const;
    friend Node<Data>* ptr_xor(Node<Data>* p, Node<Data>* q);
};

#include "XorLinkedList.tpp"

#endif //XORLINKEDLIST_H
