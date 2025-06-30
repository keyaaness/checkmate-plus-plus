#pragma once
#include "piece.hpp"

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, PieceType::ROOK) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'R' : 'r'; }
    int getValue() const override { return 500; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Rook>(color);
    }
}; 