#pragma once
#include "pieces/piece.hpp"
#include <memory>

enum class MoveType { 
    NORMAL, 
    CASTLE_KINGSIDE, 
    CASTLE_QUEENSIDE, 
    EN_PASSANT, 
    PROMOTION 
};

struct Move {
    Position from;
    Position to;
    MoveType type;
    PieceType promotionPiece;
    std::unique_ptr<Piece> capturedPiece;
    
    Move(Position f, Position t, MoveType mt = MoveType::NORMAL) 
        : from(f), to(t), type(mt), promotionPiece(PieceType::QUEEN) {}
    
    Move(const Move& other) 
        : from(other.from), to(other.to), type(other.type), 
          promotionPiece(other.promotionPiece),
          capturedPiece(other.capturedPiece ? other.capturedPiece->clone() : nullptr) {}
    
    Move& operator=(const Move& other) {
        if (this != &other) {
            from = other.from;
            to = other.to;
            type = other.type;
            promotionPiece = other.promotionPiece;
            capturedPiece = other.capturedPiece ? other.capturedPiece->clone() : nullptr;
        }
        return *this;
    }
    
    Move(Move&&) = default;
    Move& operator=(Move&&) = default;
}; 