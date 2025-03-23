// FFDH.cpp
#include "FFDH.h"

FFDH::Node::Node(int id, double first, double second)
    : id(id), data(first, second), next(nullptr) {}

FFDH::FFDH() : head(nullptr), nextId(1) {}

FFDH::~FFDH() {
    Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }
}

void FFDH::insertNode(double first, double second) {
    if (first < 0 || second < 0) {
        throw std::invalid_argument("Invalid input. Values must be non-negative.");
    }
    int id = nextId++;
    Node* newNode = new Node(id, first, second);

    if (!head || head->data.second <= second) {
        newNode->next = head;
        head = newNode;
        FFDH::PackLevels();
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
    FFDH::PackLevels();
}
void FFDH::undo() {
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
        FFDH::PackLevels();
    }
}



void FFDH::PackLevels() {
    Node* curr = head;
    shelfBottoms.clear();
    shelfHeights.clear();
    shelfUsed.clear();
    packing.clear();

    while (curr) {
        double thiswidth = curr->data.first;
        double thisheight = curr->data.second;
        // for each rectangle, we need to iterate through the shelves to see if it fits
        bool placed = false;
        for (int shelf = 0; shelf < shelfBottoms.size(); ++shelf) {
            if ((shelfHeights.size() > 0) && (shelfHeights[shelf] >= thisheight) && (shelfUsed[shelf] + thiswidth <= 1.0)) {
                // it fits! Insert into i'th shelf
                packing[shelf].push_back(std::make_tuple(curr->id, curr->data.first, curr->data.second));
                shelfUsed[shelf] += thiswidth;
                placed = true;
                break;
            }
        }
        // if it didn't fit, we make a new shelf
        if (!placed) {
            double sum = std::accumulate(shelfHeights.begin(), shelfHeights.end(), 0.0);
            shelfBottoms.push_back(sum);
            shelfHeights.push_back(thisheight);
            shelfUsed.push_back(thiswidth);
            packing.push_back({});
            packing.back().push_back(std::make_tuple(curr->id, curr->data.first, curr->data.second));
        }
        curr = curr->next;
    }
}

void FFDH::printPacking() const {
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
void FFDH::printList() const {
    Node* curr = head;
    while (curr) {
        std::cout << "(" << curr->data.first << ", " << curr->data.second << ") ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

