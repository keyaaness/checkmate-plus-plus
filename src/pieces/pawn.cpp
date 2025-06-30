#include "pawn.hpp"
#include "../board.hpp"

std::vector<Move> Pawn::getLegalMoves(const Board& board, Position pos) const {
    std::vector<Move> moves;
    int direction = isWhite() ? -1 : 1;
    int startRow = isWhite() ? 6 : 1;
    int promotionRow = isWhite() ? 0 : 7;
    
    Position oneStep(pos.row + direction, pos.col);
    if (board.getPiece(oneStep) == nullptr) {
        if (oneStep.row == promotionRow) {
            moves.emplace_back(pos, oneStep, MoveType::PROMOTION);
        } else {
            moves.emplace_back(pos, oneStep);
        }
        
        if (pos.row == startRow) {
            Position twoStep(pos.row + 2 * direction, pos.col);
            if (board.getPiece(twoStep) == nullptr) {
                moves.emplace_back(pos, twoStep);
            }
        }
    }
    
    for (int colOffset : {-1, 1}) {
        Position capturePos(pos.row + direction, pos.col + colOffset);
        if (capturePos.col >= 0 && capturePos.col < 8) {
            Piece* target = board.getPiece(capturePos);
            if (target && target->getColor() != color) {
                if (capturePos.row == promotionRow) {
                    moves.emplace_back(pos, capturePos, MoveType::PROMOTION);
                } else {
                    moves.emplace_back(pos, capturePos);
                }
            }
            
            if (board.getGameState().enPassantTarget == capturePos) {
                moves.emplace_back(pos, capturePos, MoveType::EN_PASSANT);
            }
        }
    }
    
    return moves;
} 