#pragma once
#include "piece.hpp"

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PieceType::PAWN) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'P' : 'p'; }
    int getValue() const override { return 100; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Pawn>(color);
    }
}; 