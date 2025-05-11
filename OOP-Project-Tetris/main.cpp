#include <SFML/Graphics.hpp>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
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
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual void move(int dx, int dy) = 0;
    virtual void rotate() = 0;
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

    void rotate() override {
		if (!is_active) return; // Only rotate if active
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active

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
    }
    Piece* clone() const override {
        return new I_Piece(*this); // Return a new instance of I_Piece
    }
};

//Square-piece
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
            blocks[i].setPosition(offsetX + blockX[i] * 30, offsetY + blockY[i] * 30);
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active
        // Square piece does not rotate
        cout << "Square piece does not rotate" << endl;
    }

    Piece* clone() const override {
        return new Sq_Piece(*this); // Return a new instance of Sq_Piece
    }
};


// L_Piece
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active
        int lx = blockX[1];
        int ly = blockY[1];

        rotationState = (rotationState + 1) % 4;

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
    }
    Piece* clone() const override {
        return new L_Piece(*this); // Return a new instance of L_Piece
    }
};

//J-piece
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active
        int jx = blockX[1];
        int jy = blockY[1];

        rotationState = (rotationState + 1) % 4;

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
    }
    Piece* clone() const override {
        return new J_Piece(*this); // Return a new instance of J_Piece
    }
};

//S-piece
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active
        int cx = blockX[1];
        int cy = blockY[1];

        rotationState = (rotationState + 1) % 4;

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
    }

	Piece* clone() const override {
		return new S_Piece(*this); // Return a new instance of S_Piece
  }
};

//Z-piece
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

    void rotate() override {
        if (!is_active) return; // Only rotate if active
        int zx = blockX[1];
        int zy = blockY[1];

        rotationState = (rotationState + 1) % 4;

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
    }
    Piece* clone() const override {
        return new Z_Piece(*this); // Return a new instance of Z_Piece
    }
};


class Board {
private:
    static const int rows = 20;
    static const int cols = 10;
    static const int cellSize = 30;

    sf::Color board[rows][cols];       // Each cell stores a color
    int offsetX;                       // X offset for board position
    int offsetY;                       // Y offset
    sf::RectangleShape cell;          // Used to draw each cell

public:
    Board(int x = 50, int y = 150) : offsetX(x), offsetY(y) {
        // Initialize all cells to transparent
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                board[i][j] = sf::Color::Transparent;

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
    void checkAndClearLines() {
        for (int row = 0; row < rows; ++row) {
            bool isFull = true;

            for (int col = 0; col < cols; ++col) {
                if (board[row][col] == sf::Color::Transparent) {
                    isFull = false;
                    break;
                }
            }

            if (isFull) {
                // clear full lines
                for (int r = row; r > 0; --r) {
                    for (int col = 0; col < cols; ++col) {
                        board[r][col] = board[r - 1][col];
                    }
                }

                for (int col = 0; col < cols; ++col) {
                    board[0][col] = sf::Color::Transparent;
                }

                // Check the same row again since it now contains the row above
                ++row;
            }
        }
    }
};


bool showStartScreen(sf::RenderWindow& window, sf::Font& font) {

    //gradient background
    sf::VertexArray gradient(sf::Quads, 4);

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


    sf::Text start("Press S to Start", font, 32);
    start.setFillColor(sf::Color::White);
    start.setPosition(280, 400);

    sf::Text exit("Press Esc to Exit", font, 32);
    exit.setFillColor(sf::Color::White);
    exit.setPosition(280, 500);

    sf::Font fontLogo;
    if (!fontLogo.loadFromFile("C:\\WINDOWS\\Fonts\\comicbd.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return false;
    }
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

    // Box around "Press S to Start"
    sf::FloatRect startBounds = start.getGlobalBounds();
    sf::RectangleShape boxStart(sf::Vector2f(startBounds.width + 2 * padding, startBounds.height + 2 * padding));
    boxStart.setPosition(startBounds.left - padding, startBounds.top - padding);
    boxStart.setFillColor(sf::Color::Transparent);
    boxStart.setOutlineColor(sf::Color::White);
    boxStart.setOutlineThickness(2.f);

    // Box around "Press Esc to Exit"
    sf::FloatRect exitBounds = exit.getGlobalBounds();
    sf::RectangleShape boxExit(sf::Vector2f(exitBounds.width + 2 * padding, exitBounds.height + 2 * padding));
    boxExit.setPosition(exitBounds.left - padding, exitBounds.top - padding);
    boxExit.setFillColor(sf::Color::Transparent);
    boxExit.setOutlineColor(sf::Color::White);
    boxExit.setOutlineThickness(2.f);


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
        window.draw(boxStart);
        window.draw(start);
        window.draw(boxExit);
        window.draw(exit);
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

    // Load font for Game Over text
    if (!font.loadFromFile("C:\\WINDOWS\\Fonts\\cour.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    sf::Text gameOverText("GAME OVER", font, 50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(450, 350);
   
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

            if (!isGameOver && currentPiece && event.type == sf::Event::KeyPressed) {
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
                    currentPiece->rotate();
                }
            }
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
		// Check for full lines and clear them
		if (currentPiece) {
			board.checkAndClearLines();
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
                if (board.getCell(blockY, blockX) != sf::Color::Transparent) {
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
        board.draw(window);
        if (currentPiece && !isGameOver)
            currentPiece->draw(window);

        if (isGameOver) {
            window.draw(gameOverText);
        }
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