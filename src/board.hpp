#pragma once
#include "pieces/piece.hpp"
#include "move.hpp"
#include <array>
#include <vector>
#include <memory>

struct GameState {
    Color currentPlayer;
    bool whiteCanCastleKingside;
    bool whiteCanCastleQueenside;
    bool blackCanCastleKingside;
    bool blackCanCastleQueenside;
    Position enPassantTarget;
    int halfmoveClock;
    int fullmoveNumber;
    
    GameState() : currentPlayer(Color::WHITE), 
                  whiteCanCastleKingside(true), whiteCanCastleQueenside(true),
                  blackCanCastleKingside(true), blackCanCastleQueenside(true),
                  enPassantTarget(-1, -1), halfmoveClock(0), fullmoveNumber(1) {}
};

class Board {
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> squares;
    GameState state;
    std::vector<Move> moveHistory;
    std::vector<GameState> stateHistory;
    
    bool isValidPosition(const Position& pos) const;
    bool isPathClear(const Position& from, const Position& to) const;
    Position findKing(Color color) const;
    void updateCastlingRights(const Move& move);
    void updateEnPassant(const Move& move);
    std::vector<Move> getPseudoLegalAttacks(Color color) const;
    
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    Board(Board&&) = default;
    Board& operator=(Board&&) = default;
    
    Piece* getPiece(const Position& pos) const;
    void setPiece(const Position& pos, std::unique_ptr<Piece> piece);
    std::unique_ptr<Piece> removePiece(const Position& pos);
    
    bool makeMove(const Move& move);
    void undoMove();
    
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    bool isSquareAttacked(const Position& pos, Color attackingColor) const;
    
    std::vector<Move> getLegalMoves(Color color) const;
    std::vector<Move> getPseudoLegalMoves(Color color) const;
    
    Color getCurrentPlayer() const { return state.currentPlayer; }
    const GameState& getGameState() const { return state; }
    
    void display() const;
    void setupInitialPosition();
}; 