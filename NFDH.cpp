// NFDH.cpp
#include "NFDH.h"

NFDH::Node::Node(int id, double first, double second)
    : id(id), data(first, second), next(nullptr) {}

NFDH::NFDH() : head(nullptr), nextId(1) {}

NFDH::~NFDH() {
    Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }
}

void NFDH::insertNode(double first, double second) {
    if (first < 0 || second < 0) {
        throw std::invalid_argument("Invalid input. Values must be non-negative.");
    }

    int id = nextId++;
    Node* newNode = new Node(id, first, second);

    if (!head || head->data.second <= second) {
        newNode->next = head;
        head = newNode;
        NFDH::PackLevels();
        return;
    }

    Node* prev = nullptr;
    Node* curr = head;

    while (curr && curr->data.second > second) {
        prev = curr;
        curr = curr->next;
    }

    newNode->next = curr;
    if (prev) {
        prev->next = newNode;
    } else {
        head = newNode;
    }
    NFDH::PackLevels();
}
void NFDH::undo() {
    if (head) {
        Node* prev = nullptr;
        Node* curr = head;
        while (curr && curr->id != nextId-1) {
            prev = curr;
            curr = curr->next;
        }
        // current node is the one we want to delete
        // prev is the previous node
        if (prev) {
            prev->next = curr->next;
        } else {
            head = curr->next;
        }
        delete curr;
        nextId--;
        NFDH::PackLevels();
    }
}

void NFDH::PackLevels() {
    Node* curr = head;
    shelfBottoms.clear();
    packing.clear();
    shelfBottoms.push_back(0.0);
    double sumWidth = 0.0;
    double maxHeightInShelf = 0.0;
    packing.push_back({}); // Start with an empty level

    while (curr) {
        if (sumWidth + curr->data.first >= 1.0) {
            shelfBottoms.push_back(maxHeightInShelf + shelfBottoms.back());
            packing.push_back({});
            maxHeightInShelf = 0.0;
            sumWidth = 0.0;
        }
        if (maxHeightInShelf < curr->data.second) {
            maxHeightInShelf = curr->data.second;
        }

        packing.back().push_back(std::make_tuple(curr->id, curr->data.first, curr->data.second));
        sumWidth += curr->data.first;

        curr = curr->next;
    }
}

void NFDH::printPacking() const {
    for (size_t i = 0; i < packing.size(); ++i) {
        std::cout << "Level " << i << " (Bottom: " << shelfBottoms[i] << "): ";
        for (const auto& rect : packing[i]) {
            std::cout << "(ID: " << std::get<0>(rect)
                      << ", Width: " << std::get<1>(rect)
                      << ", Height: " << std::get<2>(rect) << ") ";
        }
        std::cout << std::endl;
    }
}

void NFDH::printList() const {
    Node* curr = head;
    while (curr) {
        std::cout << "(" << curr->data.first << ", " << curr->data.second << ") ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

