#pragma once
#include "piece.hpp"

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, PieceType::KNIGHT) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'N' : 'n'; }
    int getValue() const override { return 320; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Knight>(color);
    }
}; 