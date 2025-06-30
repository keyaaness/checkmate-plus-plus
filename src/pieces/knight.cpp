#include "knight.hpp"
#include "../board.hpp"

std::vector<Move> Knight::getLegalMoves(const Board& board, Position pos) const {
    std::vector<Move> moves;
    
    int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        Position newPos(pos.row + knightMoves[i][0], pos.col + knightMoves[i][1]);
        
        if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8) {
            Piece* target = board.getPiece(newPos);
            if (target == nullptr || target->getColor() != color) {
                moves.emplace_back(pos, newPos);
            }
        }
    }
    
    return moves;
} 