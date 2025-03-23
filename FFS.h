// FFS.h
#ifndef FFS_H
#define FFS_H

#include <cmath>
#include <vector>
#include <utility> // For std::pair
#include <iostream>
#include <stdexcept>

class FFS {
private:
    double logR(double x);
    int getK(double x);
    int rectID;
    double globalFloor;
public:
    // Define a vector of vectors of pairs of doubles (the packing)
    std::vector<std::vector<std::tuple<int, double, double>>> packing;
    // Vector with the bottom level of each shelf
    std::vector<double> shelfBottoms;
    std::vector<int> shelfKs;
    std::vector<double> shelfUsed;

    double r;

    FFS(double r);

    ~FFS();

    void insertNode(double first, double second);
    void undo();

    // Function to print the packing
    void printPacking() const;

};

#endif // FFS_H

