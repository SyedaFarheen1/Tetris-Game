#pragma once
#include <SFML/Graphics.hpp>

class Board {
private:
    sf::RectangleShape boardOutline;

public:
    Board();
    void draw(sf::RenderWindow& window);
};