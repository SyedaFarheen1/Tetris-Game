#include <iostream>
#include "SFML/Graphics.hpp"
using namespace std;

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

// Base class: Piece
class Piece {
protected:
    int blockX[4];
    int blockY[4];
    sf::Color color;

public:
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void move(int dx, int dy) = 0;
    virtual void rotate() = 0;
    virtual ~Piece() {}
};

// Derived class: T_Piece
class T_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    T_Piece() {
        color = sf::Color::Magenta;
        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
        blockX[2] = 6; blockY[2] = 0;
        blockX[3] = 5; blockY[3] = 1;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(blockX[i] * 30, blockY[i] * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate() override {
        // Rotation logic can be added later based on rotationState
        cout << "Rotating T piece" << endl;
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(300, 600), "Tetris Game");

    Piece* currentPiece = new T_Piece();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Example input
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    currentPiece->move(-1, 0);
                else if (event.key.code == sf::Keyboard::Right)
                    currentPiece->move(1, 0);
                else if (event.key.code == sf::Keyboard::Down)
                    currentPiece->move(0, 1);
                else if (event.key.code == sf::Keyboard::Up)
                    currentPiece->rotate();
            }
        }

        window.clear(sf::Color::Black);
        currentPiece->draw(window);
        window.display();
    }

    delete currentPiece;
    return 0;
}
