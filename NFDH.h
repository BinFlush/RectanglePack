// NFDH.h
#ifndef NFDH_H
#define NFDH_H

#include <vector>
#include <utility> // For std::pair
#include <iostream>
#include <stdexcept>

class NFDH {
private:
    // Define a structure for a node in the linked list
    struct Node {
        int id;
        std::pair<double, double> data;
        Node* next;

        // Constructor to initialize a node
        Node(int id, double first, double second);
    };

    Node* head; // Pointer to the head of the linked list
    int nextId;

public:
    // Define a vector of vectors of pairs of doubles (the packing)
    std::vector<std::vector<std::tuple<int, double, double>>> packing;
    // Vector with the bottom level of each shelf
    std::vector<double> shelfBottoms;

    // Constructor to initialize the linked list
    NFDH();

    // Destructor to free allocated memory
    ~NFDH();

    // Function to insert a node in decreasing order based on the second double
    void insertNode(double first, double second);
    void undo();

    // Function to pack levels
    void PackLevels();

    // Function to print the packing
    void printPacking() const;

    // Function to print the linked list
    void printList() const;
};

#endif // NFDH_H

