#include "board.hpp"
#include "pieces/pawn.hpp"
#include "pieces/rook.hpp"
#include "pieces/knight.hpp"
#include "pieces/bishop.hpp"
#include "pieces/queen.hpp"
#include "pieces/king.hpp"
#include <iostream>
#include <algorithm>

Board::Board() {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            squares[i][j] = nullptr;
        }
    }
    setupInitialPosition();
}

Board::Board(const Board& other) : state(other.state), stateHistory(other.stateHistory) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (other.squares[i][j]) {
                squares[i][j] = other.squares[i][j]->clone();
            } else {
                squares[i][j] = nullptr;
            }
        }
    }
    
    for (const auto& move : other.moveHistory) {
        Move copiedMove(move.from, move.to, move.type);
        copiedMove.promotionPiece = move.promotionPiece;
        if (move.capturedPiece) {
            copiedMove.capturedPiece = move.capturedPiece->clone();
        }
        moveHistory.push_back(std::move(copiedMove));
    }
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        state = other.state;
        stateHistory = other.stateHistory;
        
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (other.squares[i][j]) {
                    squares[i][j] = other.squares[i][j]->clone();
                } else {
                    squares[i][j] = nullptr;
                }
            }
        }
        
        moveHistory.clear();
        for (const auto& move : other.moveHistory) {
            Move copiedMove(move.from, move.to, move.type);
            copiedMove.promotionPiece = move.promotionPiece;
            if (move.capturedPiece) {
                copiedMove.capturedPiece = move.capturedPiece->clone();
            }
            moveHistory.push_back(std::move(copiedMove));
        }
    }
    return *this;
}

bool Board::isValidPosition(const Position& pos) const {
    return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
}

Piece* Board::getPiece(const Position& pos) const {
    if (!isValidPosition(pos)) return nullptr;
    return squares[pos.row][pos.col].get();
}

void Board::setPiece(const Position& pos, std::unique_ptr<Piece> piece) {
    if (isValidPosition(pos)) {
        squares[pos.row][pos.col] = std::move(piece);
    }
}

std::unique_ptr<Piece> Board::removePiece(const Position& pos) {
    if (!isValidPosition(pos)) return nullptr;
    return std::move(squares[pos.row][pos.col]);
}

bool Board::isPathClear(const Position& from, const Position& to) const {
    int deltaRow = to.row - from.row;
    int deltaCol = to.col - from.col;
    
    int stepRow = (deltaRow > 0) ? 1 : (deltaRow < 0) ? -1 : 0;
    int stepCol = (deltaCol > 0) ? 1 : (deltaCol < 0) ? -1 : 0;
    
    Position current(from.row + stepRow, from.col + stepCol);
    
    while (!(current == to)) {
        if (getPiece(current) != nullptr) {
            return false;
        }
        current.row += stepRow;
        current.col += stepCol;
    }
    
    return true;
}

Position Board::findKing(Color color) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = getPiece(Position(i, j));
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(i, j);
            }
        }
    }
    return Position(-1, -1);
}

std::vector<Move> Board::getPseudoLegalAttacks(Color color) const {
    std::vector<Move> moves;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = getPiece(Position(i, j));
            if (piece && piece->getColor() == color) {
                if (piece->getType() == PieceType::KING) {
                    int directions[8][2] = {
                        {-1, -1}, {-1, 0}, {-1, 1},
                        {0, -1},           {0, 1},
                        {1, -1},  {1, 0},  {1, 1}
                    };
                    
                    for (int d = 0; d < 8; d++) {
                        Position newPos(i + directions[d][0], j + directions[d][1]);
                        if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8) {
                            moves.emplace_back(Position(i, j), newPos);
                        }
                    }
                } else {
                    auto pieceMoves = piece->getLegalMoves(*this, Position(i, j));
                    moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
                }
            }
        }
    }
    
    return moves;
}

bool Board::isSquareAttacked(const Position& pos, Color attackingColor) const {
    auto attacks = getPseudoLegalAttacks(attackingColor);
    for (const auto& move : attacks) {
        if (move.to == pos) {
            return true;
        }
    }
    return false;
}

bool Board::isInCheck(Color color) const {
    Position kingPos = findKing(color);
    if (kingPos.row == -1) return false;
    
    Color oppositeColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareAttacked(kingPos, oppositeColor);
}

std::vector<Move> Board::getPseudoLegalMoves(Color color) const {
    std::vector<Move> moves;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = getPiece(Position(i, j));
            if (piece && piece->getColor() == color) {
                auto pieceMoves = piece->getLegalMoves(*this, Position(i, j));
                moves.insert(moves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }
    
    return moves;
}

std::vector<Move> Board::getLegalMoves(Color color) const {
    std::vector<Move> pseudoMoves = getPseudoLegalMoves(color);
    std::vector<Move> legalMoves;
    
    for (const auto& move : pseudoMoves) {
        Board tempBoard = *this;
        if (tempBoard.makeMove(move) && !tempBoard.isInCheck(color)) {
            legalMoves.push_back(move);
        }
    }
    
    return legalMoves;
}

void Board::updateCastlingRights(const Move& move) {
    Piece* piece = getPiece(move.from);
    if (!piece) return;
    
    if (piece->getType() == PieceType::KING) {
        if (piece->isWhite()) {
            state.whiteCanCastleKingside = false;
            state.whiteCanCastleQueenside = false;
        } else {
            state.blackCanCastleKingside = false;
            state.blackCanCastleQueenside = false;
        }
    }
    
    if (piece->getType() == PieceType::ROOK) {
        if (piece->isWhite()) {
            if (move.from == Position(7, 0)) state.whiteCanCastleQueenside = false;
            if (move.from == Position(7, 7)) state.whiteCanCastleKingside = false;
        } else {
            if (move.from == Position(0, 0)) state.blackCanCastleQueenside = false;
            if (move.from == Position(0, 7)) state.blackCanCastleKingside = false;
        }
    }
}

void Board::updateEnPassant(const Move& move) {
    state.enPassantTarget = Position(-1, -1);
    
    Piece* piece = getPiece(move.to);
    if (piece && piece->getType() == PieceType::PAWN) {
        if (abs(move.to.row - move.from.row) == 2) {
            state.enPassantTarget = Position((move.from.row + move.to.row) / 2, move.from.col);
        }
    }
}

bool Board::makeMove(const Move& move) {
    if (!isValidPosition(move.from) || !isValidPosition(move.to)) {
        return false;
    }
    
    Piece* piece = getPiece(move.from);
    if (!piece) return false;
    
    stateHistory.push_back(state);
    moveHistory.push_back(move);
    
    Move& mutableMove = const_cast<Move&>(move);
    mutableMove.capturedPiece = removePiece(move.to);
    
    if (move.type == MoveType::CASTLE_KINGSIDE) {
        if (piece->isWhite()) {
            setPiece(Position(7, 6), removePiece(Position(7, 4)));
            setPiece(Position(7, 5), removePiece(Position(7, 7)));
        } else {
            setPiece(Position(0, 6), removePiece(Position(0, 4)));
            setPiece(Position(0, 5), removePiece(Position(0, 7)));
        }
    } else if (move.type == MoveType::CASTLE_QUEENSIDE) {
        if (piece->isWhite()) {
            setPiece(Position(7, 2), removePiece(Position(7, 4)));
            setPiece(Position(7, 3), removePiece(Position(7, 0)));
        } else {
            setPiece(Position(0, 2), removePiece(Position(0, 4)));
            setPiece(Position(0, 3), removePiece(Position(0, 0)));
        }
    } else if (move.type == MoveType::EN_PASSANT) {
        int captureRow = piece->isWhite() ? move.to.row + 1 : move.to.row - 1;
        mutableMove.capturedPiece = removePiece(Position(captureRow, move.to.col));
        setPiece(move.to, removePiece(move.from));
    } else if (move.type == MoveType::PROMOTION) {
        removePiece(move.from);
        switch (move.promotionPiece) {
            case PieceType::QUEEN:
                setPiece(move.to, std::make_unique<Queen>(piece->getColor()));
                break;
            case PieceType::ROOK:
                setPiece(move.to, std::make_unique<Rook>(piece->getColor()));
                break;
            case PieceType::BISHOP:
                setPiece(move.to, std::make_unique<Bishop>(piece->getColor()));
                break;
            case PieceType::KNIGHT:
                setPiece(move.to, std::make_unique<Knight>(piece->getColor()));
                break;
            default:
                setPiece(move.to, std::make_unique<Queen>(piece->getColor()));
                break;
        }
    } else {
        setPiece(move.to, removePiece(move.from));
    }
    
    updateCastlingRights(move);
    updateEnPassant(move);
    
    state.currentPlayer = (state.currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (state.currentPlayer == Color::WHITE) {
        state.fullmoveNumber++;
    }
    
    return true;
}

void Board::undoMove() {
    if (moveHistory.empty()) return;
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    state = stateHistory.back();
    stateHistory.pop_back();
    
    if (lastMove.type == MoveType::CASTLE_KINGSIDE) {
        Color color = getPiece(lastMove.to)->getColor();
        if (color == Color::WHITE) {
            setPiece(Position(7, 4), removePiece(Position(7, 6)));
            setPiece(Position(7, 7), removePiece(Position(7, 5)));
        } else {
            setPiece(Position(0, 4), removePiece(Position(0, 6)));
            setPiece(Position(0, 7), removePiece(Position(0, 5)));
        }
    } else if (lastMove.type == MoveType::CASTLE_QUEENSIDE) {
        Color color = getPiece(lastMove.to)->getColor();
        if (color == Color::WHITE) {
            setPiece(Position(7, 4), removePiece(Position(7, 2)));
            setPiece(Position(7, 0), removePiece(Position(7, 3)));
        } else {
            setPiece(Position(0, 4), removePiece(Position(0, 2)));
            setPiece(Position(0, 0), removePiece(Position(0, 3)));
        }
    } else if (lastMove.type == MoveType::EN_PASSANT) {
        setPiece(lastMove.from, removePiece(lastMove.to));
        Color capturedColor = (getPiece(lastMove.from)->getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE;
        int captureRow = capturedColor == Color::WHITE ? lastMove.to.row + 1 : lastMove.to.row - 1;
        setPiece(Position(captureRow, lastMove.to.col), std::move(lastMove.capturedPiece));
    } else {
        setPiece(lastMove.from, removePiece(lastMove.to));
        if (lastMove.capturedPiece) {
            setPiece(lastMove.to, std::move(lastMove.capturedPiece));
        }
    }
}

bool Board::isCheckmate(Color color) const {
    return isInCheck(color) && getLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) const {
    return !isInCheck(color) && getLegalMoves(color).empty();
}

void Board::display() const {
    std::cout << "\n  a b c d e f g h\n";
    for (int i = 0; i < 8; i++) {
        std::cout << (8 - i) << " ";
        for (int j = 0; j < 8; j++) {
            Piece* piece = getPiece(Position(i, j));
            char symbol = piece ? piece->getSymbol() : '.';
            std::cout << symbol << " ";
        }
        std::cout << (8 - i) << "\n";
    }
    std::cout << "  a b c d e f g h\n\n";
}

void Board::setupInitialPosition() {
    for (int i = 0; i < 8; i++) {
        setPiece(Position(1, i), std::make_unique<Pawn>(Color::BLACK));
        setPiece(Position(6, i), std::make_unique<Pawn>(Color::WHITE));
    }
    
    setPiece(Position(0, 0), std::make_unique<Rook>(Color::BLACK));
    setPiece(Position(0, 1), std::make_unique<Knight>(Color::BLACK));
    setPiece(Position(0, 2), std::make_unique<Bishop>(Color::BLACK));
    setPiece(Position(0, 3), std::make_unique<Queen>(Color::BLACK));
    setPiece(Position(0, 4), std::make_unique<King>(Color::BLACK));
    setPiece(Position(0, 5), std::make_unique<Bishop>(Color::BLACK));
    setPiece(Position(0, 6), std::make_unique<Knight>(Color::BLACK));
    setPiece(Position(0, 7), std::make_unique<Rook>(Color::BLACK));
    
    setPiece(Position(7, 0), std::make_unique<Rook>(Color::WHITE));
    setPiece(Position(7, 1), std::make_unique<Knight>(Color::WHITE));
    setPiece(Position(7, 2), std::make_unique<Bishop>(Color::WHITE));
    setPiece(Position(7, 3), std::make_unique<Queen>(Color::WHITE));
    setPiece(Position(7, 4), std::make_unique<King>(Color::WHITE));
    setPiece(Position(7, 5), std::make_unique<Bishop>(Color::WHITE));
    setPiece(Position(7, 6), std::make_unique<Knight>(Color::WHITE));
    setPiece(Position(7, 7), std::make_unique<Rook>(Color::WHITE));
} 