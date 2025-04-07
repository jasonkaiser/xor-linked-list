//
// Created by aldin on 18/03/2025.
//

#ifndef NODE_H
#define NODE_H

template<typename Data>
struct Node {
    Data data;
    Node<Data>* both{};
};

#endif //NODE_H
