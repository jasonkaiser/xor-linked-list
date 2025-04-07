//
// Created by aldin on 18/03/2025.
//

#include "../include/XorLinkedList.h"
#include <stdexcept>
#include <iostream>


template<typename T>
void printList(XorLinkedList<T>& list) {
    std::cout << "List (" << list.count() << " elements): ";
    for (const auto& item : list) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

int main() {
    XorLinkedList<int> list;


    list.add_to_front(3);
    list.add_to_front(2);
    list.add_to_front(1);

    printList(list);


    list.add_to_back(4);
    list.add_to_back(5);

    printList(list);


    list.add_to_front(0);
    list.add_to_front(-1);

    printList(list);


    std::cout << "Element at index 3: " << list[3] << "\n";
    std::cout << "Element at index 0: " << list[0] << "\n";
    std::cout << "Last element: " << list[list.count()-1] << "\n\n";


    std::cout << "Reversing the list...\n";
    list.reverse();
    printList(list);

    return 0;
}