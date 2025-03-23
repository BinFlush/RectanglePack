#include <SFML/Graphics.hpp>
#include <utility> // For std::pair
#include <vector>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <optional> // Required for SFML 3 event handling
#include <algorithm> // For std::clamp

#include "NFDH.h"
#include "FFDH.h"
#include "NFS.h"
#include "FFS.h"

// Function to insert rectangles into all algorithms
void insertAll(double width, double height, NFDH& nfdh, FFDH& ffdh, NFS& nfs, FFS& ffs) {
    nfdh.insertNode(width, height);
    ffdh.insertNode(width, height);
    nfs.insertNode(width, height);
    ffs.insertNode(width, height);
}

// Function to generate the r sequence
std::vector<double> generateRSequence(double r) {
    std::vector<double> sequence;
    double threshold = 1.0 / 250.0;

    // Calculate the number of terms needed
    int numTerms = static_cast<int>(std::floor(std::log(threshold) / std::log(r)));

    // Generate the sequence
    for (int i = 0; i <= numTerms; ++i) {
        sequence.push_back(std::pow(r, i));
    }

    return sequence;
}
// Function to draw packing visualization for a given algorithm
void drawPacking(
        sf::RenderWindow& window, 
        const std::vector<std::vector<std::tuple<int, double, double>>>& packing,
        const std::vector<double>& shelfBottoms,
        float offsetX,
        sf::Font& font,
        const std::string& algname) {

    //float scale = 600.0f; // Scale factor to fit in window
    float scale = 250.0f; // Scale factor to fit in window
    float boxWidth = 250.0f; // Width of each algorithm display
    float boxHeight = 600.0f; // Height of algorithm display

    // Draw bounding rectangle (open at the top)
    sf::RectangleShape boundingBox(sf::Vector2f(boxWidth, boxHeight));
    boundingBox.setPosition(sf::Vector2f(offsetX, 20));
    boundingBox.setFillColor(sf::Color::Transparent);
    boundingBox.setOutlineColor(sf::Color::White);
    boundingBox.setOutlineThickness(3);
    window.draw(boundingBox);

    // Draw identifier text
    sf::Text algtitle(font, algname);
    algtitle.setCharacterSize(15);
    algtitle.setFillColor(sf::Color::White);
    algtitle.setPosition(sf::Vector2f(offsetX+boxWidth/2-8.,boxHeight+20.));
    window.draw(algtitle);
    // Draw shelves as horizontal lines
    for (double shelfY : shelfBottoms) {
        sf::RectangleShape shelf(sf::Vector2f(boxWidth, 2));
        shelf.setPosition(sf::Vector2f(offsetX, boxHeight - shelfY * scale + 20));
        shelf.setFillColor(sf::Color::Blue);
        window.draw(shelf);
    }
    
    // Draw rectangles
    for (size_t i = 0; i < packing.size(); ++i) {
        float shelfY = shelfBottoms[i];

        double horizontalStack = 0.0;
        for (const auto& rect : packing[i]) {
            float xOffset = offsetX+scale*horizontalStack; // Track x position within bin
            
            int id = std::get<0>(rect);
            double width = std::get<1>(rect);
            double height = std::get<2>(rect);
            horizontalStack += width;

            // Convert to SFML coordinates
            float rectWidth = width * scale;
            float rectHeight = height * scale;
            float posY = boxHeight - (shelfY + height) * scale + 20; // Invert Y axis

            // Create rectangle
            sf::RectangleShape sfRect(sf::Vector2f(rectWidth, rectHeight));
            sfRect.setPosition(sf::Vector2f(xOffset, posY));
            sfRect.setFillColor(sf::Color(100, 200, 250)); // Light blue
            sfRect.setOutlineColor(sf::Color::Black);
            sfRect.setOutlineThickness(1);
            window.draw(sfRect);
            // Draw ID text inside the rectangle
            sf::Text textID(font, std::to_string(id));
            // MAGICNUMBER BAD :(
            textID.setCharacterSize(15);
            textID.setFillColor(sf::Color::White);
            textID.setPosition(sf::Vector2f((xOffset+rectWidth/2-6.),posY+rectHeight/2-10.));
            window.draw(textID);
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./main r (where r is the packing ratio sharply between 0 and 1)" << std::endl;
        return -1;
    }
    double r = std::stod(argv[1]);
    if (r >= 1.0 || r <= 0.0) {
        std::cerr << "Invalid value for r. Must be between 0 and 1." << std::endl;
        return -1;
    }
    // Initialize rectangle packing algorithms
    NFDH nfdh;
    FFDH ffdh;
    NFS nfs(r);
    FFS ffs(r);


    // Create SFML window for visualization
    sf::RenderWindow window(sf::VideoMode({1130, 650}), "Rectangle Pack");
    sf::Font font;
    if (!font.openFromFile("Arimo-Regular.ttf")) { // SFML 3 uses openFromFile
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    

    // DRAWING WINDOW
    sf::RenderWindow drawWindow(sf::VideoMode({250, 250}), "DrawWindow");
    bool drawing = false;
    sf::Vector2i startPoint, endPoint, runningPoint;
    sf::RectangleShape inputRect(sf::Vector2f(0, 0));
    inputRect.setOutlineThickness(1);
    inputRect.setOutlineColor(sf::Color::White);
    inputRect.setFillColor(sf::Color::Transparent);
    std::vector<double> rDelimiters = generateRSequence(r);
    bool undoing = false;

    // DRAWING VARIABLES DONE



    while (window.isOpen() && drawWindow.isOpen()){
        // Correctly handle the optional event
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
         // Handle events for the drawing window
        while (const std::optional drawEvent = drawWindow.pollEvent()) {
            drawWindow.clear(sf::Color(30, 30, 30)); // Dark background
                                                     //
            // Draw rDelimiters on the drawing window
            for (double rDelimiter : rDelimiters) {
                sf::RectangleShape rectangle(sf::Vector2f(250.f, 1.0f));
                rectangle.setPosition(sf::Vector2f(0.f, (1.f-rDelimiter)*250.0f));
                rectangle.setFillColor(sf::Color(100, 200, 250)); // Light blue
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setOutlineThickness(1);
                drawWindow.draw(rectangle);
            }
            if (drawEvent->is<sf::Event::Closed>()) {
                drawWindow.close();
            } else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                if (!drawing) {
                    startPoint = sf::Mouse::getPosition(drawWindow);
                    startPoint.x = std::clamp(startPoint.x, 0, 250);
                    startPoint.y = std::clamp(startPoint.y, 0, 250);
                    drawing = true;
                }
            } else if (drawing && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                endPoint = sf::Mouse::getPosition(drawWindow);
                endPoint.x = std::clamp(endPoint.x, 0, 250);
                endPoint.y = std::clamp(endPoint.y, 0, 250);
                drawing = false;
                // Calculate width and height, but keep a minimum size to avoid bugs.
                float drawnX = std::max(std::abs(endPoint.x - startPoint.x) / 250.0f, 0.01f);
                float drawnY = std::max(std::abs(endPoint.y - startPoint.y) / 250.0f, 0.01f);
                insertAll(drawnX, drawnY, nfdh, ffdh, nfs, ffs);
                
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)) {

                if (!undoing) {
                ffdh.undo();
                nfdh.undo();
                nfs.undo();
                ffs.undo();
                undoing = true;
                }
            } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Backspace)) {
                undoing = false;
            }
            if (drawing) {
                runningPoint = sf::Mouse::getPosition(drawWindow);
                runningPoint.x = std::clamp(runningPoint.x, 0, 250);
                runningPoint.y = std::clamp(runningPoint.y, 0, 250);
                sf::RectangleShape rectangle(sf::Vector2f(std::abs(runningPoint.x - startPoint.x), std::abs(runningPoint.y - startPoint.y)));
                //sf::RectangleShape rectangle(sf::Vector2f(20.f, 20.f));
                rectangle.setPosition(sf::Vector2f(std::min(startPoint.x, runningPoint.x), std::min(startPoint.y, runningPoint.y)));
                rectangle.setFillColor(sf::Color(100, 200, 250)); // Light blue
                rectangle.setOutlineColor(sf::Color::Black);
                rectangle.setOutlineThickness(1);
                drawWindow.draw(rectangle);
            } //else {
            //}
        }
        // Draw rectangle in drawWindow
        window.clear(sf::Color(30, 30, 30)); // Dark background

        // Draw packing results for each algorithm
        drawPacking(window, nfdh.packing, nfdh.shelfBottoms, 20, font, "NFDH");    // NFDH
        drawPacking(window, ffdh.packing, ffdh.shelfBottoms, 300, font, "FFDH");   // FFDH
        drawPacking(window, nfs.packing, nfs.shelfBottoms, 580, font, "NFS");     // NFS
        drawPacking(window, ffs.packing, ffs.shelfBottoms, 860, font, "FFS");     // FFS

        window.display();
        drawWindow.display();
    }
    return 0;
}

