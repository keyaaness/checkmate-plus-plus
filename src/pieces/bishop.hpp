#pragma once
#include "piece.hpp"

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, PieceType::BISHOP) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'B' : 'b'; }
    int getValue() const override { return 330; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Bishop>(color);
    }
}; 