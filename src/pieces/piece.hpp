#pragma once
#include <vector>
#include <memory>

enum class Color { WHITE, BLACK };
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };

struct Position {
    int row, col;
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
};

class Board;
struct Move;

class Piece {
protected:
    Color color;
    PieceType type;
    
public:
    Piece(Color c, PieceType t) : color(c), type(t) {}
    virtual ~Piece() = default;
    
    virtual std::vector<Move> getLegalMoves(const Board& board, Position pos) const = 0;
    virtual char getSymbol() const = 0;
    virtual int getValue() const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;
    
    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    bool isWhite() const { return color == Color::WHITE; }
}; 