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
};

//Square-piece
class Sq_Piece : public Piece {
private:
    sf::RectangleShape blocks[4];
    int rotationState; // O piece doesn’t rotate, but keeping for consistency

public:
    Sq_Piece() {
        color = sf::Color::Yellow;

        // A 2x2 square in the top-middle of the board
        blockX[0] = 4; blockY[0] = 0;
        blockX[1] = 5; blockY[1] = 0;
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
        cout << "square piece does not rotate" << endl;
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


bool showStartScreen(sf::RenderWindow& window, sf::Font& font) {
    sf::Text start("Press S to Start", font, 32);
    start.setFillColor(sf::Color::Yellow);
    start.setPosition(280, 250);

    sf::Text exit("Press Esc to Exit", font, 32);
    exit.setFillColor(sf::Color::Red);
    exit.setPosition(280, 320);

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
        window.draw(start);
        window.draw(exit);
        window.display();
    }
    return false;
}

void runGameLoop(sf::RenderWindow& window, sf::Font& font) {

    const int numPieces = 7;
    Piece* pieces[numPieces] = {
        new T_Piece(), new I_Piece(), new Sq_Piece(), new L_Piece(),
        new J_Piece(), new S_Piece(), new Z_Piece()
    };

    Piece* currentPiece = nullptr;
    sf::Clock dropClock;
    float dropDelay = 0.5f;
    Board board;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle movement only for current active piece
            if (currentPiece && event.type == sf::Event::KeyPressed) {
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

        // Spawn new piece if none is active
        if (!currentPiece) {
            int idx = rand() % numPieces;
            currentPiece = pieces[idx];
            currentPiece->setOffset(50, 150);
            currentPiece->setActive(true);
            dropClock.restart();
        }

        // Auto-drop the active piece
        if (currentPiece && dropClock.getElapsedTime().asSeconds() >= dropDelay) {
            currentPiece->move(0, 1); // Move down
            dropClock.restart();

            // Simple bottom check — stop at bottom row (adjust based on your grid size)
            bool atBottom = false;
            for (int i = 0; i < 4; ++i) {
                int yBlock = currentPiece->getY(i) + currentPiece->getOffsetY() / 30;
                if (yBlock >= 19) {  // 20 rows = index 0 to 19
                    atBottom = true;
                    break;
                }
            }

            if (atBottom) {
                currentPiece->setActive(false);
                currentPiece = nullptr;
            }
        }

        // Draw everything
        window.clear(sf::Color::Black);
        board.draw(window);
        for (int i = 0; i < numPieces; ++i) {
            pieces[i]->draw(window);
            window.display();
        }

    }

    // Cleanup
    for (int i = 0; i < numPieces; ++i)
        delete pieces[i];

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

