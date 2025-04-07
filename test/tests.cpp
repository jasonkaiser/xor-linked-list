//
// Created by aldin on 18/03/2025.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include "../include/XorLinkedList.h"
#include <stdexcept>
#include <string>

TEST_CASE("Add to front and iterate") {
    XorLinkedList<int> list;
    CHECK_EQ(list.get_head(), nullptr);

    // Add elements to front: list becomes 1 -> 2 -> 3.
    list.add_to_front(3);
    list.add_to_front(2);
    list.add_to_front(1);

    int expected[] = {1, 2, 3};
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        CHECK_EQ(list.get(i), expected[index]);
        ++index;
    }
    CHECK_EQ(index, 3);

    // Check indexing operator.
    CHECK_EQ(list[0], 1);
    CHECK_EQ(list[1], 2);
    CHECK_EQ(list[2], 3);
    CHECK_THROWS_AS(list[3], std::out_of_range);
}

TEST_CASE("Add to back and iterate") {
    XorLinkedList<int> list;
    // Build list: 1 -> 2 -> 3 using add_to_back.
    list.add_to_back(1);
    list.add_to_back(2);
    list.add_to_back(3);

    // Verify order using get() and indexing operator.
    CHECK_EQ(list.get(0), 1);
    CHECK_EQ(list.get(1), 2);
    CHECK_EQ(list.get(2), 3);
    CHECK_EQ(list[0], 1);
    CHECK_EQ(list[1], 2);
    CHECK_EQ(list[2], 3);

    // Verify iterator traversal.
    int expected[] = {1, 2, 3};
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        CHECK_EQ(list.get(i), expected[index]);
        ++index;
    }
    CHECK_EQ(index, 3);
}

TEST_CASE("XOR pointer integrity for add_to_front") {
    XorLinkedList<int> list;
    list.add_to_front(20); // Single node.
    Node<int>* head = list.get_head();
    CHECK_NE(head, nullptr);
    CHECK_EQ(head->data, 20);
    // For a single node, next should be nullptr.
    CHECK_EQ(head->both, nullptr);

    // Add another node to the front: new list becomes 10 -> 20.
    list.add_to_front(10);
    head = list.get_head();
    CHECK_NE(head, nullptr);
    CHECK_EQ(head->data, 10);
    // The head's next should be the pointer to the second node.
    Node<int>* second = head->both;
    CHECK_NE(second, nullptr);
    CHECK_EQ(second->data, 20);
}

TEST_CASE("XOR pointer integrity for add_to_back") {
    XorLinkedList<int> list;
    list.add_to_back(100); // Single node.
    Node<int>* head = list.get_head();
    CHECK_NE(head, nullptr);
    CHECK_EQ(head->data, 100);
    CHECK_EQ(head->both, nullptr);

    // Add another node to the back: list becomes 100 -> 200.
    list.add_to_back(200);
    head = list.get_head();
    Node<int>* second = head->both;
    CHECK_NE(second, nullptr);
    CHECK_EQ(second->data, 200);

    // For the second node, its next pointer stores XOR(pointer to first, nullptr).
    // Recovering the "next" from the second node: XOR(first, second->both) should be nullptr.
    Node<int>* recovered = ptr_xor(head, second->both);
    CHECK_EQ(recovered, nullptr);
}

TEST_CASE("Remove from front operations") {
    XorLinkedList<int> list;
    // Build list: 10 -> 20 -> 30 using add_to_back.
    list.add_to_back(10);
    list.add_to_back(20);
    list.add_to_back(30);

    // Remove the front element (10). New list should be 20 -> 30.
    list.remove_from_front();
    // Check that head is now 20.
    Node<int>* head = list.get_head();
    CHECK_NE(head, nullptr);
    CHECK_EQ(head->data, 20);

    int expected[] = {20, 30};
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        CHECK_EQ(list.get(i), expected[index]);
        ++index;
    }
    CHECK_EQ(index, 2);

    // Verify pointer integrity:
    // For a list 20 -> 30, head->both should point to the second node.
    Node<int>* second = head->both;
    CHECK_NE(second, nullptr);
    CHECK_EQ(second->data, 30);
    // For tail node (30), its next field stores XOR(pointer to previous, nullptr).
    // Recovering the previous pointer should give head.
    Node<int>* recovered = ptr_xor(head, second->both);
    CHECK_EQ(recovered, nullptr); // Since tail's next should be XOR(head, nullptr) == head.
}

TEST_CASE("Remove from back operations") {
    XorLinkedList<int> list;
    // Build list: 10 -> 20 -> 30 using add_to_back.
    list.add_to_back(10);
    list.add_to_back(20);
    list.add_to_back(30);

    // Remove from back. After removal, list should become 10 -> 20.
    list.remove_from_back();
    int expected1[] = {10, 20};
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        CHECK_EQ(list.get(i), expected1[index]);
        ++index;
    }
    CHECK_EQ(index, 2);

    // Check pointer integrity:
    // Head should still be 10.
    Node<int>* head = list.get_head();
    CHECK_EQ(head->data, 10);
    // head->both should point to the new tail (20).
    Node<int>* second = head->both;
    CHECK_NE(second, nullptr);
    CHECK_EQ(second->data, 20);
    // The new tail's next should be XOR(pointer to previous, nullptr). Recovering gives nullptr.
    Node<int>* recovered = ptr_xor(head, second->both);
    CHECK_EQ(recovered, nullptr);

    // Remove the remaining element from back.
    list.remove_from_back(); // Now list becomes just [10].
    head = list.get_head();
    CHECK_NE(head, nullptr);
    CHECK_EQ(head->data, 10);
    CHECK_EQ(head->both, nullptr);

    // Remove the final element.
    list.remove_from_back(); // List should be empty.
    CHECK_EQ(list.get_head(), nullptr);
    CHECK_THROWS_AS(list.remove_from_back(), std::out_of_range);
}

TEST_CASE("Iterate using the Iterator class") {
    XorLinkedList<int> list;
    // Build list: 50 -> 60 -> 70 using add_to_back.
    list.add_to_back(50);
    list.add_to_back(60);
    list.add_to_back(70);

    int expected1[] = {50, 60, 70};
    int index = 0;

    for (auto i : list) {
        CHECK_EQ(i, expected1[index]);
        ++index;
    }
}

TEST_CASE("Copy constructor creates an independent deep copy") {
    XorLinkedList<int> original;
    original.add_to_back(1);
    original.add_to_back(2);
    original.add_to_back(3);

    XorLinkedList<int> copy = original; // Copy constructor

    CHECK_EQ(copy.count(), 3);
    CHECK_EQ(copy.get(0), 1);
    CHECK_EQ(copy.get(1), 2);
    CHECK_EQ(copy.get(2), 3);

    // Ensure deep copy by modifying original
    original.add_to_back(4);
    CHECK_EQ(original.count(), 4);
    CHECK_EQ(copy.count(), 3); // Copy should remain unchanged

    original.remove_from_front();
    CHECK_EQ(original.get(0), 2);
    CHECK_EQ(copy.get(0), 1); // Copy should remain unchanged
}

TEST_CASE("Copy assignment correctly copies and clears existing list") {
    XorLinkedList<int> original;
    original.add_to_back(10);
    original.add_to_back(20);
    original.add_to_back(30);

    XorLinkedList<int> copy;
    copy.add_to_back(100);
    copy.add_to_back(200);

    copy = original; // Copy assignment

    CHECK_EQ(copy.count(), 3);
    CHECK_EQ(copy.get(0), 10);
    CHECK_EQ(copy.get(1), 20);
    CHECK_EQ(copy.get(2), 30);

    // Ensure deep copy
    original.add_to_back(40);
    CHECK_EQ(original.count(), 4);
    CHECK_EQ(copy.count(), 3);

    original.remove_from_front();
    CHECK_EQ(original.get(0), 20);
    CHECK_EQ(copy.get(0), 10); // Copy should remain unchanged
}

TEST_CASE("Move constructor transfers ownership and clears source") {
    XorLinkedList<int> original;
    original.add_to_back(5);
    original.add_to_back(6);
    original.add_to_back(7);

    XorLinkedList<int> moved = std::move(original); // Move constructor

    CHECK_EQ(moved.count(), 3);
    CHECK_EQ(moved.get(0), 5);
    CHECK_EQ(moved.get(1), 6);
    CHECK_EQ(moved.get(2), 7);

    // Original list should be empty after move
    CHECK_EQ(original.count(), 0);
    CHECK_EQ(original.get_head(), nullptr);
}

TEST_CASE("Move assignment transfers ownership and clears source") {
    XorLinkedList<int> original;
    original.add_to_back(50);
    original.add_to_back(60);
    original.add_to_back(70);

    XorLinkedList<int> moved;
    moved.add_to_back(100);
    moved.add_to_back(200);

    moved = std::move(original); // Move assignment

    CHECK_EQ(moved.count(), 3);
    CHECK_EQ(moved.get(0), 50);
    CHECK_EQ(moved.get(1), 60);
    CHECK_EQ(moved.get(2), 70);

    // Original list should be empty after move
    CHECK_EQ(original.count(), 0);
    CHECK_EQ(original.get_head(), nullptr);
}

TEST_CASE("Initializer list constructor correctly initializes list") {
    XorLinkedList<int> list = {10, 20, 30, 40};

    CHECK_EQ(list.count(), 4);
    CHECK_EQ(list.get(0), 10);
    CHECK_EQ(list.get(1), 20);
    CHECK_EQ(list.get(2), 30);
    CHECK_EQ(list.get(3), 40);

    // Ensure correct iteration order
    int expected[] = {10, 20, 30, 40};
    int index = 0;
    for (int i = 0; i < list.count(); i++) {
        CHECK_EQ(list.get(i), expected[index]);
        ++index;
    }
    CHECK_EQ(index, 4);
}
