#pragma once
#include "piece.hpp"

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, PieceType::QUEEN) {}
    
    std::vector<Move> getLegalMoves(const Board& board, Position pos) const override;
    char getSymbol() const override { return isWhite() ? 'Q' : 'q'; }
    int getValue() const override { return 900; }
    std::unique_ptr<Piece> clone() const override {
        return std::make_unique<Queen>(color);
    }
}; 