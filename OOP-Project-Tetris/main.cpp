#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

// Base class: Piece
class Piece {
protected:
    int blockX[4];
    int blockY[4];
    sf::Color color;
    int offsetX = 50;
    int offsetY = 150;

public:
    void setOffset(int x, int y) {
        offsetX = x;
        offsetY = y;
    }
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
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
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

        rotationState = (rotationState + 1) % 4;

        int x = blockX[1];
        int y = blockY[1];

        if (rotationState == 0) {

            blockX[0] = x - 1;   blockY[0] = y;
            blockX[1] = x;       blockY[1] = y;
            blockX[2] = x + 1;   blockY[2] = y;
            blockX[3] = x;       blockY[3] = y + 1;

        }
        else if (rotationState == 1) {
            blockX[0] = x;  blockY[0] = y-1;
            blockX[1] = x;      blockY[1] = y;
            blockX[2] = x;  blockY[2] = y+1;
            blockX[3] = x-1;      blockY[3] = y;
            
        }
        else if (rotationState == 2) {
            blockX[0] = x - 1; blockY[0] = y;
            blockX[1] = x;     blockY[1] = y;
            blockX[2] = x + 1; blockY[2] = y;
            blockX[3] = x;     blockY[3] = y - 1;
        }
        else if (rotationState == 3) {
            blockX[0] = x;     blockY[0] = y - 1;
            blockX[1] = x;     blockY[1] = y;
            blockX[2] = x;     blockY[2] = y + 1;
            blockX[3] = x + 1; blockY[3] = y;
        }

    }
};


class Board {
private:
    static const int rows = 20;
    static const int cols = 10;
    static const int cellSize = 30;

    int board[rows][cols];               // Main game grid
    int offsetX;                         // X offset for board position
    int offsetY;                         // Y offset
    sf::RectangleShape cell;            // Used to draw each cell

public:
    // Constructor with optional offsets
    Board(int x = 50, int y = 150) : offsetX(x), offsetY(y) {
        // Initialize board with empty cells
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                board[i][j] = 0;

        cell.setSize(sf::Vector2f(cellSize, cellSize));
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color(80, 80, 80));
    }

    // Draw the board: boundaries + empty cells
    void draw(sf::RenderWindow& window) {
        // Draw boundaries (gray border)
        for (int row = 0; row < rows + 2; row++) {
            for (int col = 0; col < cols + 2; col++) {
                if (row == 0 || row == rows + 1 || col == 0 || col == cols + 1) {
                    cell.setPosition(offsetX + col * cellSize, offsetY + row * cellSize);
                    cell.setFillColor(sf::Color(128, 128, 128)); // Gray
                    window.draw(cell);
                }
            }
        }

        // Draw grid cells
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                cell.setPosition(offsetX + (col + 1) * cellSize, offsetY + (row + 1) * cellSize);
                if (board[row][col] == 0)
                    cell.setFillColor(sf::Color::Black); // Empty cell
                else
                    cell.setFillColor(sf::Color::White); // Filled cell (temporary for now)

                window.draw(cell);
            }
        }
    }
};




int main() {
    sf::RenderWindow window(sf::VideoMode(900, 800), "Tetris Game");

    Piece* currentPiece = new T_Piece();
    Board board;

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
        board.draw(window);         // Draw board with boundaries
        currentPiece->draw(window);
        window.display();
    }

    delete currentPiece;
    return 0;
}
