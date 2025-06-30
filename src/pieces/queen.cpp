#include "queen.hpp"
#include "../board.hpp"

std::vector<Move> Queen::getLegalMoves(const Board& board, Position pos) const {
    std::vector<Move> moves;
    
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (int d = 0; d < 8; d++) {
        int deltaRow = directions[d][0];
        int deltaCol = directions[d][1];
        
        for (int i = 1; i < 8; i++) {
            Position newPos(pos.row + i * deltaRow, pos.col + i * deltaCol);
            
            if (newPos.row < 0 || newPos.row >= 8 || newPos.col < 0 || newPos.col >= 8) {
                break;
            }
            
            Piece* target = board.getPiece(newPos);
            if (target == nullptr) {
                moves.emplace_back(pos, newPos);
            } else {
                if (target->getColor() != color) {
                    moves.emplace_back(pos, newPos);
                }
                break;
            }
        }
    }
    
    return moves;
} 