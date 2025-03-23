// NFS.cpp
#include "NFS.h"


NFS::NFS(double r) : r(r), rectID(1), globalFloor(0.0) {}

NFS::~NFS() {}

double NFS::logR(double x) {
    return std::log(x) / std::log(r);
}
int NFS::getK(double x) {
    return std::floor(NFS::logR(x));
}

void NFS::insertNode(double first, double second) {
    bool placed = false;
    int k = NFS::getK(second);

    // reverse search for top shelf with k
    for (int i = shelfKs.size() - 1; i >= 0; --i) {
        if (shelfKs[i] == k) {
            if (shelfUsed[i] + first <= 1.0) {
                placed = true;
                shelfUsed[i] += first;
                packing[i].push_back(std::make_tuple(rectID, first, second));
                placed = true;
            }
            // for this alg, we only check the last shelf of appropriate k
            break;
        }
    }
    if (!placed) {
        // rectangle did not get placed. Make new shelf
        packing.push_back({});
        packing.back().push_back(std::make_tuple(rectID, first, second));
        shelfKs.push_back(k);
        shelfUsed.push_back(first);
        shelfBottoms.push_back(globalFloor);
        globalFloor += std::pow(r, k);
    }
    rectID++;

}
void NFS::undo() {
    // latest rectangle added to packing is last element of one of the rows
    for (int i = 0; i < packing.size(); ++i) {
        int j = packing[i].size() - 1;
        if (std::get<0>(packing[i][j]) == rectID-1) {
            if (j == 0) {
                // this shelf has only one rectangle. Delete the shelf and its rectangle
                // (it has to be the last shelf)
                int thisK = shelfKs[i];
                globalFloor -= std::pow(r, thisK);
                packing[i].clear();
                packing.pop_back();
                shelfBottoms.pop_back();
                shelfUsed.pop_back();
                shelfKs.pop_back();
                rectID--;
                return;
            } else {
                // this shelf has more than one rectangle. Delete the last rectangle
                // but it can be any shelf
                double used = std::get<1>(packing[i][j]);
                shelfUsed[i] -= used;
                packing[i].pop_back();
                rectID--;
                return;
            }
        }
    }
}
                



void NFS::printPacking() const {
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


