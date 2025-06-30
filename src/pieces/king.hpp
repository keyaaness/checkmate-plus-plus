#pragma once
#include "piece.hpp"

class King : public Piece {
public:
    King(Color c) : Piece(c, PieceType::KING) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'K' : 'k'; }
    int getValue() const override { return 20000; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<King>(color);
    }
}; 