#include "game.hpp"
#include "player.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Game::Game() : result(GameResult::ONGOING) {}

void Game::setPlayers(std::unique_ptr<Player> white, std::unique_ptr<Player> black) {
    whitePlayer = std::move(white);
    blackPlayer = std::move(black);
}

Position Game::parsePosition(const std::string& posStr) const {
    if (posStr.length() != 2) return Position(-1, -1);
    
    char file = posStr[0];
    char rank = posStr[1];
    
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8') {
        return Position(-1, -1);
    }
    
    return Position(8 - (rank - '0'), file - 'a');
}

std::string Game::positionToString(const Position& pos) const {
    if (pos.row < 0 || pos.row >= 8 || pos.col < 0 || pos.col >= 8) {
        return "";
    }
    
    char file = 'a' + pos.col;
    char rank = '8' - pos.row;
    
    return std::string(1, file) + std::string(1, rank);
}

bool Game::parseMove(const std::string& moveStr, Move& move) const {
    if (moveStr.length() < 4 || moveStr.length() > 5) return false;
    
    Position from = parsePosition(moveStr.substr(0, 2));
    Position to = parsePosition(moveStr.substr(2, 2));
    
    if (from.row == -1 || to.row == -1) return false;
    
    move = Move(from, to);
    
    if (moveStr.length() == 5) {
        char promotion = moveStr[4];
        switch (promotion) {
            case 'q': case 'Q': move.promotionPiece = PieceType::QUEEN; break;
            case 'r': case 'R': move.promotionPiece = PieceType::ROOK; break;
            case 'b': case 'B': move.promotionPiece = PieceType::BISHOP; break;
            case 'n': case 'N': move.promotionPiece = PieceType::KNIGHT; break;
            default: return false;
        }
        move.type = MoveType::PROMOTION;
    }
    
    return true;
}

std::string Game::moveToString(const Move& move) const {
    std::string result = positionToString(move.from) + positionToString(move.to);
    
    if (move.type == MoveType::PROMOTION) {
        switch (move.promotionPiece) {
            case PieceType::QUEEN: result += 'q'; break;
            case PieceType::ROOK: result += 'r'; break;
            case PieceType::BISHOP: result += 'b'; break;
            case PieceType::KNIGHT: result += 'n'; break;
            default: result += 'q'; break;
        }
    }
    
    return result;
}

void Game::displayBoard() const {
    board.display();
}

void Game::displayGameInfo() const {
    std::cout << "Current player: " << (board.getCurrentPlayer() == Color::WHITE ? "White" : "Black") << "\n";
    
    if (board.isInCheck(board.getCurrentPlayer())) {
        std::cout << "Check!\n";
    }
    
    switch (result) {
        case GameResult::WHITE_WINS:
            std::cout << "White wins!\n";
            break;
        case GameResult::BLACK_WINS:
            std::cout << "Black wins!\n";
            break;
        case GameResult::DRAW:
            std::cout << "Draw!\n";
            break;
        case GameResult::ONGOING:
            break;
    }
}

void Game::playMove(const Move& move) {
    if (board.makeMove(move)) {
        Color currentPlayer = board.getCurrentPlayer();
        
        if (board.isCheckmate(currentPlayer)) {
            result = (currentPlayer == Color::WHITE) ? GameResult::BLACK_WINS : GameResult::WHITE_WINS;
        } else if (board.isStalemate(currentPlayer)) {
            result = GameResult::DRAW;
        }
    }
}

void Game::run() {
    if (!whitePlayer || !blackPlayer) {
        std::cout << "Players not set!\n";
        return;
    }
    
    std::cout << "Checkmate++ Chess Engine\n";
    std::cout << "Enter moves in format: e2e4 (from-to)\n";
    std::cout << "Type 'quit' to exit\n\n";
    
    while (result == GameResult::ONGOING) {
        displayBoard();
        displayGameInfo();
        
        Player* currentPlayer = (board.getCurrentPlayer() == Color::WHITE) ? whitePlayer.get() : blackPlayer.get();
        
        try {
            Move move = currentPlayer->getMove(board);
            
            std::vector<Move> legalMoves = board.getLegalMoves(board.getCurrentPlayer());
            bool isLegal = false;
            
            for (const auto& legalMove : legalMoves) {
                if (move.from == legalMove.from && move.to == legalMove.to) {
                    if (move.type == MoveType::PROMOTION) {
                        if (legalMove.type == MoveType::PROMOTION) {
                            move.type = legalMove.type;
                            isLegal = true;
                            break;
                        }
                    } else {
                        move.type = legalMove.type;
                        isLegal = true;
                        break;
                    }
                }
            }
            
            if (isLegal) {
                std::cout << "Move: " << moveToString(move) << "\n\n";
                playMove(move);
            } else {
                std::cout << "Illegal move!\n";
            }
            
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    
    displayBoard();
    displayGameInfo();
} 