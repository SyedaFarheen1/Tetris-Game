#pragma once
#include <SFML/Graphics.hpp>

const int ROWS = 20;
const int COLS = 10;
const int BLOCK_SIZE = 30; 

class Board {
private:
    int grid[ROWS][COLS];
    sf::RectangleShape cell;

public:
    Board();
    void draw(sf::RenderWindow& window);
    void setBlock(int row, int col, sf::Color color); 
};