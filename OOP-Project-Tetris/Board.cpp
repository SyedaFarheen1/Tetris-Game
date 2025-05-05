#include "Board.h"
#include "Piece.h"

Board::Board() {
   
    for (int i = 0; i < ROWS; ++i)
        for (int j = 0; j < COLS; ++j)
            grid[i][j] = 0;  // Initialize grid to 0 (empty)
}

