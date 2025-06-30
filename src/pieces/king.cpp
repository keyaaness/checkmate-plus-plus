#include "king.hpp"
#include "../board.hpp"

std::vector<Move> King::getLegalMoves(const Board& board, Position pos) const {
    std::vector<Move> moves;
    
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };
    
    for (int i = 0; i < 8; i++) {
        Position newPos(pos.row + directions[i][0], pos.col + directions[i][1]);
        
        if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8) {
            Piece* target = board.getPiece(newPos);
            if (target == nullptr || target->getColor() != color) {
                moves.emplace_back(pos, newPos);
            }
        }
    }
    
    const GameState& state = board.getGameState();
    Color oppositeColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    if (!board.isInCheck(color)) {
        if (isWhite()) {
            if (state.whiteCanCastleKingside && 
                board.getPiece(Position(7, 5)) == nullptr &&
                board.getPiece(Position(7, 6)) == nullptr &&
                !board.isSquareAttacked(Position(7, 5), oppositeColor) &&
                !board.isSquareAttacked(Position(7, 6), oppositeColor)) {
                moves.emplace_back(pos, Position(7, 6), MoveType::CASTLE_KINGSIDE);
            }
            
            if (state.whiteCanCastleQueenside &&
                board.getPiece(Position(7, 1)) == nullptr &&
                board.getPiece(Position(7, 2)) == nullptr &&
                board.getPiece(Position(7, 3)) == nullptr &&
                !board.isSquareAttacked(Position(7, 2), oppositeColor) &&
                !board.isSquareAttacked(Position(7, 3), oppositeColor)) {
                moves.emplace_back(pos, Position(7, 2), MoveType::CASTLE_QUEENSIDE);
            }
        } else {
            if (state.blackCanCastleKingside &&
                board.getPiece(Position(0, 5)) == nullptr &&
                board.getPiece(Position(0, 6)) == nullptr &&
                !board.isSquareAttacked(Position(0, 5), oppositeColor) &&
                !board.isSquareAttacked(Position(0, 6), oppositeColor)) {
                moves.emplace_back(pos, Position(0, 6), MoveType::CASTLE_KINGSIDE);
            }
            
            if (state.blackCanCastleQueenside &&
                board.getPiece(Position(0, 1)) == nullptr &&
                board.getPiece(Position(0, 2)) == nullptr &&
                board.getPiece(Position(0, 3)) == nullptr &&
                !board.isSquareAttacked(Position(0, 2), oppositeColor) &&
                !board.isSquareAttacked(Position(0, 3), oppositeColor)) {
                moves.emplace_back(pos, Position(0, 2), MoveType::CASTLE_QUEENSIDE);
            }
        }
    }
    
    return moves;
} 