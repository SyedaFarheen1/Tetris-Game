#include <SFML/Graphics.hpp>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <iostream>
using namespace std;

class Board {
private:
    static const int rows = 20;
    static const int cols = 10;
    static const int cellSize = 30;

    sf::Color board[rows][cols];       // Each cell stores a color
    int offsetX;                       // X offset for board position
    int offsetY;                       // Y offset
    sf::RectangleShape cell;          // Used to draw each cell

    int score;
    int linesCleared;
    int level;

public:
    Board(int x = 50, int y = 150) : offsetX(x), offsetY(y) {
        // Initialize all cells to transparent
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                board[i][j] = sf::Color::Transparent;
        }
        score = 0;
        level = 1;
        linesCleared = 0;
        cell.setSize(sf::Vector2f(cellSize, cellSize));
        cell.setOutlineThickness(1);
        cell.setOutlineColor(sf::Color(80, 80, 80));
    }

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

        // Draw filled grid cells
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                cell.setPosition(offsetX + (col + 1) * cellSize, offsetY + (row + 1) * cellSize);
                if (board[row][col] != sf::Color::Transparent)
                    cell.setFillColor(board[row][col]);
                else
                    cell.setFillColor(sf::Color::Black); // Empty cell

                window.draw(cell);
            }
        }
    }
    sf::Color getCell(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols)
            return board[row][col];
        return sf::Color::Black; // Invalid 
    }

    void setCell(int row, int col, sf::Color color) {
        if (row >= 0 && row < rows && col >= 0 && col < cols)
            board[row][col] = color;
    }
    void setScore(int condition) {
        if (condition == 1) {
            score += 40 * (level + 1);
        }
        else if (condition == 2) {
            score += 100 * (level + 1);
        }
        else if (condition == 3) {
            score += 300 * (level + 1);
        }
        else if (condition == 4) {
            score += 1200 * (level + 1);
        }
        else {
            score += 0;
        }
    }
    void setLinesCleared(int lines) {
        linesCleared += lines;
    }
    int getLinesCleared() const {
        return linesCleared;
    }
    int getScore() const {
        return score;
    }
    int getLevel() const {
        return level;
    }
    void setLevel(int newLevel) {
        level = newLevel;
    }
    int checkAndClearLines() {
        int linesClearedThisMove = 0; // Track lines cleared in this move

        for (int row = 0; row < rows; ++row) {
            bool isFull = true;

            for (int col = 0; col < cols; ++col) {
                if (board[row][col] == sf::Color::Transparent) {
                    isFull = false;
                    break;
                }
            }

            if (isFull) {
                ++linesClearedThisMove; // Increment for this move
                ++linesCleared;         // Increment total lines cleared

                // Clear the full line and shift rows above down
                for (int r = row; r > 0; --r) {
                    for (int col = 0; col < cols; ++col) {
                        board[r][col] = board[r - 1][col];
                    }
                }

                // Clear the top row
                for (int col = 0; col < cols; ++col) {
                    board[0][col] = sf::Color::Transparent;
                }

                // Check the same row again since it now contains the row above
                --row;
            }
        }

        return linesClearedThisMove; // Return lines cleared in this move
    }
};

// Base class: Piece
class Piece {
protected:
    int blockX[4] = { 0 };
    int blockY[4] = { 0 };
    sf::Color color;
    int offsetX = 50;
    int offsetY = 150;
    bool is_active = false;
public:
    void setActive(bool val) { 
        is_active = val; 
    }
    bool isActive() const { 
        return is_active; 
    }
    void setOffset(int x, int y) {
        offsetX = x;
        offsetY = y;
    }
	int getX(int index) const {
		return blockX[index];
	}
	int getY(int index) const {
		return blockY[index];
	}
	int getOffsetX() const {
		return offsetX;
	}
	int getOffsetY() const {
		return offsetY;
	}
    bool isValidPosition(const Board& board) const {
        for (int i = 0; i < 4; ++i) {
            int x = blockX[i];
            int y = blockY[i];

            // Check if the block is out of bounds
            if (x < 0 || x >= 10 || y < 0 || y >= 20) {
                return false;
            }

            // Check if the block overlaps with an existing block on the board
            if (board.getCell(y, x) != sf::Color::Transparent) {
                return false;
            }
        }
        return true;
    }
    bool tryWallKick(Board& board) {
        // Define possible shifts for wall kicks
        const int shifts[4][2] = {
            {1, 0},  // Right
            {-1, 0}, // Left
            {0, -1}, // Down,
			{0, 1}   // Up
        };

        // Try each of the 4 possible shifts
        for (int s = 0; s < 4; ++s) {
            int shiftX = shifts[s][0];
            int shiftY = shifts[s][1];

            // Apply the shift to each block
            for (int i = 0; i < 4; ++i) {
                blockX[i] += shiftX;
                blockY[i] += shiftY;
            }

            // Check if the new position is valid
            if (isValidPosition(board)) {
                return true; // Wall kick succeeded
            }

            // Undo the shift
            for (int i = 0; i < 4; ++i) {
                blockX[i] -= shiftX;
                blockY[i] -= shiftY;
            }
        }

        return false; // No valid wall kick found
    }
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void move(int dx, int dy) = 0;
    virtual void rotate(Board& board) = 0; // Pure virtual method with Board& parameter
    virtual Piece* clone() const = 0;  // Pure virtual clone
    virtual sf::Color getColor() const { return color; }
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
		if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
		if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
		if (!is_active) return; // Only rotate if active
        
        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
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
            blockX[0] = x;      blockY[0] = y-1;
            blockX[1] = x;      blockY[1] = y;
            blockX[2] = x;      blockY[2] = y+1;
            blockX[3] = x-1;    blockY[3] = y;
            
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

		// Check if the new position is valid
        if (!isValidPosition(board)) {
            // If not, try Wall Kick
            if (!tryWallKick(board)) {
                // If Wall Kick fails, revert to original position
                for (int i = 0; i < 4; ++i) {
                    blockX[i] = originalBlockX[i];
                    blockY[i] = originalBlockY[i];
                }
                rotationState = originalRotationState; // Revert rotation state
            }
        }
    }
	Piece* clone() const override {
		return new T_Piece(*this); // Return a new instance of T_Piece
	}

};

// Derived class: I_Piece
class I_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    I_Piece() {
        color = sf::Color::Cyan;

        blockX[0] = 5; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 1;
        blockX[2] = 5; blockY[2] = 2;
        blockX[3] = 5; blockY[3] = 3;


        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
		if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
		if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active

        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
        rotationState = (rotationState + 1) % 2;

        int Ix = blockX[1];
        int Iy = blockY[1];

        if (rotationState == 0) {
            blockX[0] = Ix - 1; blockY[0] = Iy;
            blockX[1] = Ix;     blockY[1] = Iy;
            blockX[2] = Ix + 1; blockY[2] = Iy;
            blockX[3] = Ix + 2; blockY[3] = Iy;
        }
        else if (rotationState == 1) {
            blockX[0] = Ix; blockY[0] = Iy - 1;
            blockX[1] = Ix; blockY[1] = Iy;
            blockX[2] = Ix; blockY[2] = Iy + 1;
            blockX[3] = Ix; blockY[3] = Iy + 2;
        }

		// Check if the new position is valid
		if (!isValidPosition(board)) {
			// If not, try Wall Kick
			if (!tryWallKick(board)) {
				// If Wall Kick fails, revert to original position
				for (int i = 0; i < 4; ++i) {
					blockX[i] = originalBlockX[i];
					blockY[i] = originalBlockY[i];
				}
				rotationState = originalRotationState; // Revert rotation state
			}
		}
    }
    Piece* clone() const override {
        return new I_Piece(*this); // Return a new instance of I_Piece
    }
};

// Derived class: Square-piece
class Sq_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState; // Square piece doesn’t rotate, but keeping for consistency

public:
    Sq_Piece() {
        color = sf::Color::Yellow;

        // A 2x2 square in the top-middle of the board
        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
        blockX[2] = 4; blockY[2] = 1;
        blockX[3] = 5; blockY[3] = 1;

        offsetX = 4 * 30;
        offsetY = 0;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30)); // Each block is 30x30 pixels
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            // Correctly align the blocks with the grid
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active
        // Square piece does not rotate
        cout << "Square piece does not rotate" << endl;
    }

    Piece* clone() const override {
        return new Sq_Piece(*this); // Return a new instance of Sq_Piece
    }
};

// Derived class: L_Piece
class L_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    L_Piece() {
        color = sf::Color(255, 165, 0); // Orange

        // Initial coordinates (L shape)
        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
        blockX[2] = 6; blockY[2] = 0;
        blockX[3] = 6; blockY[3] = 1;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active

        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
        rotationState = (rotationState + 1) % 4;

        int lx = blockX[1];
        int ly = blockY[1];

        if (rotationState == 0) {
            blockX[0] = lx;  blockY[0] = ly - 1;
            blockY[1] = lx;  blockY[1] = ly;
            blockX[2] = lx;  blockY[2] = ly + 1;
            blockX[3] = lx + 1;  blockY[3] = ly + 1;
        }
        else if (rotationState == 1) {
            blockX[0] = lx - 1;  blockY[0] = ly;
            blockX[1] = lx;      blockY[1] = ly;
            blockX[2] = lx + 1;  blockY[2] = ly;
            blockX[3] = lx + 1;  blockY[3] = ly - 1;
        }
        else if (rotationState == 2) {
            blockX[0] = lx;  blockY[0] = ly + 1;
            blockX[1] = lx;  blockY[1] = ly;
            blockX[2] = lx;  blockY[2] = ly - 1;
            blockX[3] = lx - 1;  blockY[3] = ly - 1;
        }
        else if (rotationState == 3) {
            blockX[0] = lx + 1;  blockY[0] = ly;
            blockX[1] = lx;  blockY[1] = ly;
            blockX[2] = lx - 1;  blockY[2] = ly;
            blockX[3] = lx - 1;  blockY[3] = ly + 1;
        }

		// Check if the new position is valid
        if (!isValidPosition(board)) {
            // If not, try Wall Kick
            if (!tryWallKick(board)) {
                // If Wall Kick fails, revert to original position
                for (int i = 0; i < 4; ++i) {
                    blockX[i] = originalBlockX[i];
                    blockY[i] = originalBlockY[i];
                }
                rotationState = originalRotationState; // Revert rotation state
            }
        }
    }
    Piece* clone() const override {
        return new L_Piece(*this); // Return a new instance of L_Piece
    }
};

// Derived class: J-piece
class J_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    J_Piece() {
        color = sf::Color(0, 0, 255); // Blue

        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
        blockX[2] = 6; blockY[2] = 0;
        blockX[3] = 4; blockY[3] = 1;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active

        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
        rotationState = (rotationState + 1) % 4;

        int jx = blockX[1];
        int jy = blockY[1];

        if (rotationState == 0) {
            blockX[0] = jx;  blockY[0] = jy - 1;
            blockX[1] = jx;  blockY[1] = jy;
            blockX[2] = jx;  blockY[2] = jy + 1;
            blockX[3] = jx - 1;  blockY[3] = jy + 1;
        }
        else if (rotationState == 1) {
            blockX[0] = jx + 1;  blockY[0] = jy;
            blockX[1] = jx;  blockY[1] = jy;
            blockX[2] = jx - 1;  blockY[2] = jy;
            blockX[3] = jx - 1;  blockY[3] = jy - 1;
        }
        else if (rotationState == 2) {
            blockX[0] = jx;  blockY[0] = jy + 1;
            blockX[1] = jx;  blockY[1] = jy;
            blockX[2] = jx;  blockY[2] = jy - 1;
            blockX[3] = jx + 1;  blockY[3] = jy - 1;
        }
        else if (rotationState == 3) {
            blockX[0] = jx - 1;  blockY[0] = jy;
            blockX[1] = jx;  blockY[1] = jy;
            blockX[2] = jx + 1;  blockY[2] = jy;
            blockX[3] = jx + 1;  blockY[3] = jy + 1;
        }

        // Check if the rotated position is valid
        if (!isValidPosition(board)) {
            // Try wall kick if not valid
            if (!tryWallKick(board)) {
                // If wall kick also fails, revert to the original state
                for (int i = 0; i < 4; ++i) {
                    blockX[i] = originalBlockX[i];
                    blockY[i] = originalBlockY[i];
                }
                rotationState = originalRotationState;
            }
        }
    }
    Piece* clone() const override {
        return new J_Piece(*this); // Return a new instance of J_Piece
    }
};

//Derived class: S-piece
class S_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    S_Piece() {
        color = sf::Color::Green;

        blockX[0] = 5; blockY[0] = 0;
        blockX[1] = 6; blockY[1] = 0;
        blockX[2] = 4; blockY[2] = 1;
        blockX[3] = 5; blockY[3] = 1;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active

        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
        rotationState = (rotationState + 1) % 4;

        int cx = blockX[1];
        int cy = blockY[1];

        if (rotationState == 0) {
            blockX[0] = cx + 1;  blockY[0] = cy;
            blockX[1] = cx;      blockY[1] = cy;
            blockX[2] = cx;      blockY[2] = cy + 1;
            blockX[3] = cx - 1;  blockY[3] = cy + 1;
        }
        if (rotationState == 1) {
            blockX[0] = cx;      blockY[0] = cy + 1;
            blockX[1] = cx;      blockY[1] = cy;
            blockX[2] = cx - 1;  blockY[2] = cy;
            blockX[3] = cx - 1;  blockY[3] = cy - 1;
        }
        if (rotationState == 2) {
            blockX[0] = cx - 1;  blockY[0] = cy;
            blockX[1] = cx;      blockY[1] = cy;
            blockX[2] = cx;      blockY[2] = cy - 1;
            blockX[3] = cx + 1;  blockY[3] = cy - 1;
        }
        if (rotationState == 3) {
            blockX[0] = cx;      blockY[0] = cy - 1;
            blockX[1] = cx;      blockY[1] = cy;
            blockX[2] = cx + 1;  blockY[2] = cy;
            blockX[3] = cx + 1;  blockY[3] = cy + 1;
        }
        // Check if the rotated position is valid
        if (!isValidPosition(board)) {
            // Try wall kick if not valid
            if (!tryWallKick(board)) {
                // If wall kick also fails, revert to the original state
                for (int i = 0; i < 4; ++i) {
                    blockX[i] = originalBlockX[i];
                    blockY[i] = originalBlockY[i];
                }
                rotationState = originalRotationState;
            }
        }
    }

	Piece* clone() const override {
		return new S_Piece(*this); // Return a new instance of S_Piece
    }
};

//Derived class: Z-piece
class Z_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState;

public:
    Z_Piece() {
        color = sf::Color::Red;

        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
        blockX[2] = 5; blockY[2] = 1;
        blockX[3] = 6; blockY[3] = 1;

        for (int i = 0; i < 4; i++) {
            blocks[i].setSize(sf::Vector2f(30, 30));
            blocks[i].setFillColor(color);
        }

        rotationState = 0;
    }

    void draw(sf::RenderWindow& window) override {
        if (!is_active) return; // Only draw if active
        for (int i = 0; i < 4; i++) {
            blocks[i].setPosition(offsetX + (blockX[i] + 1) * 30, offsetY + (blockY[i] + 1) * 30);
            window.draw(blocks[i]);
        }
    }

    void move(int dx, int dy) override {
        if (!is_active) return; // Only move if active
        for (int i = 0; i < 4; i++) {
            blockX[i] += dx;
            blockY[i] += dy;
        }
    }

    void rotate(Board& board) override {
        if (!is_active) return; // Only rotate if active

        // Save the current positions of the piece
        int originalBlockX[4];
        int originalBlockY[4];
        for (int i = 0; i < 4; ++i) {
            originalBlockX[i] = blockX[i];
            originalBlockY[i] = blockY[i];
        }
        int originalRotationState = rotationState;
        rotationState = (rotationState + 1) % 4;

        int zx = blockX[1];
        int zy = blockY[1];

        if (rotationState == 0) {
            blockX[0] = zx - 1;  blockY[0] = zy;
            blockX[1] = zx;      blockY[1] = zy;
            blockX[2] = zx;      blockY[2] = zy + 1;
            blockX[3] = zx + 1;  blockY[3] = zy + 1;
        }
        else if (rotationState == 1) {
            blockX[0] = zx;      blockY[0] = zy - 1;
            blockX[1] = zx;      blockY[1] = zy;
            blockX[2] = zx - 1;  blockY[2] = zy;
            blockX[3] = zx - 1;  blockY[3] = zy + 1;
        }
        else if (rotationState == 2) {
            blockX[0] = zx + 1;      blockY[0] = zy;
            blockX[1] = zx;          blockY[1] = zy;
            blockX[2] = zx;          blockY[2] = zy - 1;
            blockX[3] = zx - 1;      blockY[3] = zy - 1;
        }
        else if (rotationState == 3) {
            blockX[0] = zx;      blockY[0] = zy + 1;
            blockX[1] = zx;      blockY[1] = zy;
            blockX[2] = zx + 1;  blockY[2] = zy;
            blockX[3] = zx + 1;  blockY[3] = zy - 1;
        }

        // Check if the rotated position is valid
        if (!isValidPosition(board)) {
            // Try wall kick if not valid
            if (!tryWallKick(board)) {
                // If wall kick also fails, revert to the original state
                for (int i = 0; i < 4; ++i) {
                    blockX[i] = originalBlockX[i];
                    blockY[i] = originalBlockY[i];
                }
                rotationState = originalRotationState;
            }
        }
    }
    Piece* clone() const override {
        return new Z_Piece(*this); // Return a new instance of Z_Piece
    }
};

bool showStartScreen(sf::RenderWindow& window, sf::Font& font) {

    //gradient background
    sf::VertexArray gradient(sf::Quads, 4);

    sf::Font fontLogo;
    if (!fontLogo.loadFromFile("C:\\WINDOWS\\Fonts\\comicbd.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return false;
    }

    const int windowWidth = 1200;
    const int windowHeight = 1000;

    gradient[0].position = sf::Vector2f(0, 0);
    gradient[1].position = sf::Vector2f(windowWidth, 0);
    gradient[2].position = sf::Vector2f(windowWidth, windowHeight);
    gradient[3].position = sf::Vector2f(0, windowHeight);

    gradient[0].color = sf::Color(48, 25, 52);
    gradient[1].color = sf::Color(48, 25, 52);
    gradient[2].color = sf::Color(0, 0, 100);
    gradient[3].color = sf::Color(0, 0, 100);

    //start text
    sf::Text start("Start", fontLogo, 32);
    start.setFillColor(sf::Color::White);
    start.setPosition(260 + (400 - start.getLocalBounds().width) / 2, 430);

    // Box around start
    sf::RectangleShape startBox(sf::Vector2f(220, 60));
    startBox.setPosition(350, 420);
    startBox.setFillColor(sf::Color::Transparent);
    startBox.setOutlineThickness(3);
    startBox.setOutlineColor(sf::Color::White);
    
    //game rules text
    sf::Text gameRules("Game Rules", fontLogo, 32);
    gameRules.setFillColor(sf::Color::White);
    gameRules.setPosition(260 + (400 - gameRules.getLocalBounds().width) / 2, 500);

    // box around game rules
    sf::RectangleShape gameRule(sf::Vector2f(220, 60));
    gameRule.setPosition(350, 490);
    gameRule.setFillColor(sf::Color::Transparent);
    gameRule.setOutlineThickness(2);
    gameRule.setOutlineColor(sf::Color::White);

    //exit text
    sf::Text exit("Exit", fontLogo, 32);
    exit.setFillColor(sf::Color::White);
    exit.setPosition(260 + (400 - exit.getLocalBounds().width) / 2, 570);

    //box around exit
    sf::RectangleShape exitBox(sf::Vector2f(220, 60));
    exitBox.setPosition(350, 560);
    exitBox.setFillColor(sf::Color::Transparent);
    exitBox.setOutlineThickness(2);
    exitBox.setOutlineColor(sf::Color::White);
    
    sf::Text T("T", fontLogo, 200);
    T.setFillColor(sf::Color::Red);
    T.setPosition(100, 100);

    sf::Text E("E", fontLogo, 200);
    E.setFillColor(sf::Color(255,155,0));
    E.setPosition(220, 100);

    sf::Text Tt("T", fontLogo, 200);
    Tt.setFillColor(sf::Color::Yellow);
    Tt.setPosition(330, 100);

    sf::Text R("R", fontLogo, 200);
    R.setFillColor(sf::Color::Green);
    R.setPosition(440, 100);

    sf::Text I("I", fontLogo, 200);
    I.setFillColor(sf::Color::Blue);
    I.setPosition(550, 100);

    sf::Text S("S", fontLogo, 200);
    S.setFillColor(sf::Color::Magenta);
    S.setPosition(660, 100);


    float padding = 15.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::S) {
                    return true;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                    return false;

            }
        }

        window.clear(sf::Color::Black);
        window.draw(gradient);
        window.draw(startBox);
        window.draw(start);
        window.draw(exitBox);
        window.draw(exit);
        window.draw(gameRules);
        window.draw(gameRule);
        window.draw(T);
        window.draw(E);
        window.draw(Tt);
        window.draw(R);
        window.draw(I);
        window.draw(S);

        window.display();
    }
    return false;
}

void runGameLoop(sf::RenderWindow& window, sf::Font& font) {

    Piece* templates[7];
    templates[0] = new T_Piece();
    templates[1] = new I_Piece();
    templates[2] = new Sq_Piece();
    templates[3] = new L_Piece();
    templates[4] = new J_Piece();
    templates[5] = new S_Piece();
    templates[6] = new Z_Piece();

    Piece* currentPiece = nullptr;
    sf::Clock dropClock;
    float dropDelay = 0.5f;

    Board board;
    bool isGameOver = false;

    int score = 0;
    int highScore = 0;
    int level = 0;

    bool isPaused = false;
    int selectedOption = 0; // 0 = Resume, 1 = Restart

    sf::Font fontLogo2;
    if (!fontLogo2.loadFromFile("C:\\WINDOWS\\Fonts\\comicbd.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    //TETRIS text
    sf::Text T("T", fontLogo2, 80);
    T.setFillColor(sf::Color::Red);
    T.setPosition(300, 30);

    sf::Text E("E", fontLogo2, 80);
    E.setFillColor(sf::Color(255, 155, 0));
    E.setPosition(350, 30);

    sf::Text Tt("T", fontLogo2, 80);
    Tt.setFillColor(sf::Color::Yellow);
    Tt.setPosition(400, 30);

    sf::Text R("R", fontLogo2, 80);
    R.setFillColor(sf::Color::Green);
    R.setPosition(450, 30);

    sf::Text I("I", fontLogo2, 80);
    I.setFillColor(sf::Color::Blue);
    I.setPosition(500, 30);

    sf::Text S("S", fontLogo2, 80);
    S.setFillColor(sf::Color::Magenta);
    S.setPosition(550, 30);

    //background gradient
    sf::VertexArray gradient2(sf::Quads, 4);

    const int windowWidth = 1200;
    const int windowHeight = 1000;

    gradient2[0].position = sf::Vector2f(0, 0);
    gradient2[1].position = sf::Vector2f(windowWidth, 0);
    gradient2[2].position = sf::Vector2f(windowWidth, windowHeight);
    gradient2[3].position = sf::Vector2f(0, windowHeight);

    gradient2[0].color = sf::Color(48, 25, 52);
    gradient2[1].color = sf::Color(48, 25, 52);
    gradient2[2].color = sf::Color(0, 0, 100);
    gradient2[3].color = sf::Color(0, 0, 100);

    // pause menu box
    sf::RectangleShape pauseMenuBox(sf::Vector2f(400, 400));
    pauseMenuBox.setFillColor(sf::Color(0, 0, 0, 150));
    pauseMenuBox.setOutlineThickness(3);
    pauseMenuBox.setOutlineColor(sf::Color::White);
    pauseMenuBox.setPosition(250, 150);

    // pause title
    sf::Text pauseText("Game Paused", fontLogo2, 50);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setPosition(250 + (400 - pauseText.getLocalBounds().width) / 2, 170);

    // resume button
    sf::RectangleShape resumeBox(sf::Vector2f(220, 60));
    resumeBox.setPosition(340, 260);
    resumeBox.setFillColor(sf::Color::Transparent);
    resumeBox.setOutlineThickness(2);
    resumeBox.setOutlineColor(sf::Color::White);

    sf::Text resumeText("Resume", fontLogo2, 40);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setPosition(345 + (210 - resumeText.getLocalBounds().width) / 2, 268); // centered

    // restart button
    sf::RectangleShape restartBox(sf::Vector2f(220, 60));
    restartBox.setPosition(340, 340); 
    restartBox.setFillColor(sf::Color::Transparent);
    restartBox.setOutlineThickness(2);
    restartBox.setOutlineColor(sf::Color::White);

    sf::Text restartText("Restart", fontLogo2, 40);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(345 + (210 - restartText.getLocalBounds().width) / 2, 348); // centered

    // exit button
    sf::RectangleShape exitBox(sf::Vector2f(220, 60));
    exitBox.setPosition(340, 420);
    exitBox.setFillColor(sf::Color::Transparent);
    exitBox.setOutlineThickness(2);
    exitBox.setOutlineColor(sf::Color::White);

    sf::Text exitText("Exit", fontLogo2, 40);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(345 + (210 - exitText.getLocalBounds().width) / 2, 428); // centered


    // Load font for Game Over text
    if (!font.loadFromFile("C:\\WINDOWS\\Fonts\\cour.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    sf::Text gameOverText("GAME OVER", fontLogo2, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(490, 530);
   
    
    //display Current score
    sf::Text scoreText;
    scoreText.setFont(fontLogo2);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(630, 360);
    
    sf::Text CurrentScore("Current Score", fontLogo2, 40);
    CurrentScore.setFillColor(sf::Color::White);
    CurrentScore.setPosition(520, 300);

    //display high score
    sf::Text HighScoreText;
    HighScoreText.setFont(fontLogo2);
    HighScoreText.setCharacterSize(24);
    HighScoreText.setFillColor(sf::Color::White);
    HighScoreText.setPosition(630, 210);

    sf::Text HighScore("High Score", fontLogo2, 40);
    HighScore.setFillColor(sf::Color::White);
    HighScore.setPosition(530, 150);

    //display level
    sf::Text Level;
    Level.setFont(fontLogo2);
    Level.setCharacterSize(24);
    Level.setFillColor(sf::Color::White);
    Level.setPosition(630, 480);

    sf::Text GameLevel("Level", fontLogo2, 40);
    GameLevel.setFillColor(sf::Color::White);
    GameLevel.setPosition(585,420);

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Bag system using an array
    int bag[7];
    int bagIndex = 0;

    auto shuffleBag = [&]() {
        for (int i = 0; i < 7; ++i) {
            bag[i] = i;
        }
        for (int i = 6; i > 0; --i) {
            int j = rand() % (i + 1);
            std::swap(bag[i], bag[j]);
        }
        bagIndex = 0;
        };

    shuffleBag();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P && !isGameOver) {
                    isPaused = !isPaused; // Toggle pause state only if the game is not over
                    if (isPaused)
                        cout << "\nGame Paused!" << endl;
                    else
                        cout << "\nGame Unpaused!" << endl;
                }
                if (isPaused) {
                    // Handle navigation in pause menu
                    if (event.key.code == sf::Keyboard::Up) {
                        selectedOption = (selectedOption - 1 + 3) % 3; // Navigate up + wrap around
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        selectedOption = (selectedOption + 1) % 3; // Navigate down + wrap around
                    }
                    else if (event.key.code == sf::Keyboard::Enter) {
                        if (selectedOption == 0) {
                            cout << "\nResuming Game" << endl;
                            isPaused = false; // Resume game
                        }
                        else if (selectedOption == 1) {
                            // Restart game
                            cout << "\nGame Restarted!" << endl;
                            delete currentPiece;
                            currentPiece = nullptr;
                            board = Board(); // Reset board
                            isGameOver = false;
                            isPaused = false;
                            dropClock.restart();
                        }
						else if (selectedOption == 2) {
                            cout << "\nExiting Game! Goodbye!" << endl;
							window.close(); // Exit game
						}
                    }
                }
                else if (!isGameOver && currentPiece && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Left) {
                        bool canMoveLeft = true;
                        for (int i = 0; i < 4; ++i) {
                            int blockX = currentPiece->getX(i);
                            int blockY = currentPiece->getY(i);
                            if (blockX - 1 < 0 || board.getCell(blockY, blockX - 1) != sf::Color::Transparent) {
                                canMoveLeft = false;
                                break;
                            }
                        }
                        if (canMoveLeft)
                            currentPiece->move(-1, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        bool canMoveRight = true;
                        for (int i = 0; i < 4; ++i) {
                            int blockX = currentPiece->getX(i);
                            int blockY = currentPiece->getY(i);
                            if (blockX + 1 >= 10 || board.getCell(blockY, blockX + 1) != sf::Color::Transparent) {
                                canMoveRight = false;
                                break;
                            }
                        }
                        if (canMoveRight)
                            currentPiece->move(1, 0);
                    }
                    else if (event.key.code == sf::Keyboard::Down) {
                        bool canMoveDown = true;
                        for (int i = 0; i < 4; ++i) {
                            int blockX = currentPiece->getX(i);
                            int blockY = currentPiece->getY(i);
                            if (blockY + 1 >= 20 || board.getCell(blockY + 1, blockX) != sf::Color::Transparent) {
                                canMoveDown = false;
                                break;
                            }
                        }
                        if (canMoveDown) {
                            currentPiece->move(0, 1);
                        }
                        else {
                            for (int i = 0; i < 4; ++i) {
                                int blockX = currentPiece->getX(i);
                                int blockY = currentPiece->getY(i);
                                board.setCell(blockY, blockX, currentPiece->getColor());
                            }
                            delete currentPiece;
                            currentPiece = nullptr;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Up) {
                        currentPiece->rotate(board);
                    }
                }
            }
        }
		// Handle game pause
        if (isPaused) {
            // Update pause menu UI
            resumeBox.setOutlineColor(selectedOption == 0 ? sf::Color::Green : sf::Color::White);
            restartBox.setOutlineColor(selectedOption == 1 ? sf::Color::Green : sf::Color::White);
			exitBox.setOutlineColor(selectedOption == 2 ? sf::Color::Red : sf::Color::White);

            // Render pause menu
            window.clear(sf::Color::Black);
            window.draw(gradient2);
            window.draw(pauseMenuBox);
            window.draw(pauseText);
            window.draw(resumeBox);
            window.draw(resumeText);
            window.draw(restartBox);
            window.draw(restartText);
			window.draw(exitBox);
			window.draw(exitText);
            window.display();
            continue; // Skip the rest of the game loop while paused
        }

        if (!isGameOver && currentPiece && dropClock.getElapsedTime().asSeconds() >= dropDelay) {
            bool atBottom = false;
            for (int i = 0; i < 4; ++i) {
                int blockX = currentPiece->getX(i);
                int blockY = currentPiece->getY(i);
                if (blockY + 1 >= 20 || board.getCell(blockY + 1, blockX) != sf::Color::Transparent) {
                    atBottom = true;
                    break;
                }
            }
            if (atBottom) {
                for (int i = 0; i < 4; ++i) {
                    int blockX = currentPiece->getX(i);
                    int blockY = currentPiece->getY(i);
                    board.setCell(blockY, blockX, currentPiece->getColor());
                }
                delete currentPiece;
                currentPiece = nullptr;
            }
            else {
                currentPiece->move(0, 1);
            }

            dropClock.restart();
        }
		// Checking for full lines and clear them
        if (currentPiece) {
            int cleared = board.checkAndClearLines(); // Get lines cleared in this move
            if (cleared > 0) {
                // classic Tetris scoring system
                if (cleared == 1) {
                    board.setScore(1); // Single line
                }
                else if (cleared == 2) {
                    board.setScore(2); // Double line
                }
                else if (cleared == 3) {
                    board.setScore(3); // Triple line
                }
                else if (cleared == 4) {
                    board.setScore(4); // Tetris (4 lines)
                }

                // Level up every 10 lines
                if (board.getLinesCleared() >= board.getLevel() * 10) {
                    board.setLevel(board.getLevel() + 1);
                    dropDelay *= 0.8f; // Make blocks fall faster
                    cout << "\n\nLevel Up! Current Level: " << board.getLevel() << endl;
                }

                // Output the updated score
                cout << "\nScore: " << board.getScore() << "\nLines Cleared: " << board.getLinesCleared() << "\nLevel: " << board.getLevel() << endl;
            }
        }

       

        // Game over check during spawn
        if (!isGameOver && !currentPiece) {
            if (bagIndex >= 7)
                shuffleBag();

            int index = bag[bagIndex++];
            currentPiece = templates[index]->clone();
            currentPiece->setOffset(50, 150);
            currentPiece->setActive(true);

            // Check if any block of the new piece overlaps the board
            bool collisionAtSpawn = false;
            for (int i = 0; i < 4; ++i) {
                int blockX = currentPiece->getX(i);
                int blockY = currentPiece->getY(i);
                if (blockY >= 0 && board.getCell(blockY, blockX) != sf::Color::Transparent) {
                    collisionAtSpawn = true;
                    break;
                }
            }
            if (collisionAtSpawn) {
                std::cout << "Game Over!" << std::endl;
                delete currentPiece;
                currentPiece = nullptr;
                isGameOver = true;
            }

            dropClock.restart();
        }

        window.clear(sf::Color::Black);
        window.draw(gradient2);
        board.draw(window);

        if (currentPiece && !isGameOver)
            currentPiece->draw(window);

        if (isGameOver) {
            window.draw(gameOverText);
        }

        //Current Score
        score = board.getScore();
        level = board.getLevel();
        scoreText.setString(std::to_string(score));
        HighScoreText.setString(std::to_string(highScore));
        Level.setString(std::to_string(level));
        
        window.draw(T);
        window.draw(E);
        window.draw(Tt);
        window.draw(R);
        window.draw(I);
        window.draw(S);
        window.draw(CurrentScore);
        window.draw(scoreText);
        window.draw(HighScore);
        window.draw(HighScoreText);
        window.draw(GameLevel);
        window.draw(Level);
        window.display();
        
    }

    for (int i = 0; i < 7; ++i)
        delete templates[i];
    delete currentPiece;

}

int main() {

    srand(static_cast<unsigned>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(900, 800), "Tetris Game");

    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\lucon.ttf")) {
        std::cerr << "Could not load font.\n";
        return -1;
    }

    if (showStartScreen(window, font)){
        runGameLoop(window, font);
    }
    return 0;
}