#include <iostream>
#include "Game.h"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <array>

using namespace std;


class Tetromino {
public:
    enum class Type { I, O, T, S, Z, J, L };

    Tetromino(Type type, float x, float y)
        : type(type), position(x, y) {
        setShape(type);
        setColor(type);
    }

    void setShape(Type type) {
        switch (type) {
        case Type::I:
            shape = { {{0, 0}, {1, 0}, {2, 0}, {3, 0}} }; break;
        case Type::O:
            shape = { {{0, 0}, {1, 0}, {0, 1}, {1, 1}} }; break;
        case Type::T:
            shape = { {{0, 0}, {1, 0}, {2, 0}, {1, 1}} }; break;
        case Type::S:
            shape = { {{1, 0}, {2, 0}, {0, 1}, {1, 1}} }; break;
        case Type::Z:
            shape = { {{0, 0}, {1, 0}, {1, 1}, {2, 1}} }; break;
        case Type::J:
            shape = { {{0, 0}, {0, 1}, {1, 1}, {2, 1}} }; break;
        case Type::L:
            shape = { {{2, 0}, {0, 1}, {1, 1}, {2, 1}} }; break;
        }
    }

    void setColor(Type type) {
        switch (type) {
        case Type::I: color = sf::Color::Cyan; break;
        case Type::O: color = sf::Color::Yellow; break;
        case Type::T: color = sf::Color::Magenta; break;
        case Type::S: color = sf::Color::Green; break;
        case Type::Z: color = sf::Color::Red; break;
        case Type::J: color = sf::Color::Blue; break;
        case Type::L: color = sf::Color(255, 165, 0); break; // Orange
        }
    }

    void rotate() {
        for (auto& block : shape) {
            int temp = block.x;
            block.x = block.y;
            block.y = -temp;
        }
    }

    void move(float dx, float dy) {
        position.x += dx;
        position.y += dy;
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& block : shape) {
            sf::RectangleShape square(sf::Vector2f(blockSize - 1, blockSize - 1));
            square.setPosition(position.x + block.x * blockSize, position.y + block.y * blockSize);
            square.setFillColor(color);
            window.draw(square);
        }
    }

private:
    Type type;
    sf::Vector2f position;
    sf::Color color;
    std::array<sf::Vector2i, 4> shape;
    static constexpr float blockSize = 30.f;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(300, 600), "Tetromino with SFML");
    window.setFramerateLimit(60);

    // Create Tetrominos
    Tetromino I(Tetromino::Type::I, 90.f, 90.f);
    Tetromino O(Tetromino::Type::O, 90.f, 90.f);
    Tetromino T(Tetromino::Type::T, 90.f, 90.f);
    Tetromino S(Tetromino::Type::S, 90.f, 90.f);
    Tetromino Z(Tetromino::Type::Z, 90.f, 90.f);
    Tetromino J(Tetromino::Type::J, 90.f, 90.f);
    Tetromino L(Tetromino::Type::L, 90.f, 90.f);

    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left)
                    I.move(-30.f, 0.f);
                else if (event.key.code == sf::Keyboard::Right)
                    I.move(30.f, 0.f);
                else if (event.key.code == sf::Keyboard::Down)
                    I.move(0.f, 30.f);
                else if (event.key.code == sf::Keyboard::Up)
                    I.rotate();
            }
            

        }

        window.clear(sf::Color::Black);
        O.draw(window);
        I.draw(window);
        
        window.display();
    }


	return 0;
}