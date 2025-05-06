#include "Board.h"
#include "Piece.h"

const int ROWS = 20;
const int COLS = 10;
const int BLOCK_SIZE = 30;

Board::Board() {
    int width = COLS * BLOCK_SIZE;
    int height = ROWS * BLOCK_SIZE;

    boardOutline.setSize(sf::Vector2f(width - 2, height - 2)); // -2 for visible border
    boardOutline.setPosition(1, 1); // slight offset
    boardOutline.setFillColor(sf::Color::Transparent);
    boardOutline.setOutlineThickness(2);
    boardOutline.setOutlineColor(sf::Color::White);
}

void Board::draw(sf::RenderWindow& window) {
    window.draw(boardOutline);
}
