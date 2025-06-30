#include "player.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

Move HumanPlayer::getMove(const Board& board) {
    std::string input;
    std::cout << (color == Color::WHITE ? "White" : "Black") << " to move: ";
    std::getline(std::cin, input);
    
    if (input == "quit") {
        exit(0);
    }
    
    if (input.length() < 4) {
        throw std::runtime_error("Invalid move format");
    }
    
    Position from, to;
    
    if (input[0] >= 'a' && input[0] <= 'h' && input[1] >= '1' && input[1] <= '8') {
        from = Position(8 - (input[1] - '0'), input[0] - 'a');
    } else {
        throw std::runtime_error("Invalid source position");
    }
    
    if (input[2] >= 'a' && input[2] <= 'h' && input[3] >= '1' && input[3] <= '8') {
        to = Position(8 - (input[3] - '0'), input[2] - 'a');
    } else {
        throw std::runtime_error("Invalid destination position");
    }
    
    Move move(from, to);
    
    if (input.length() >= 5) {
        char promotion = input[4];
        switch (promotion) {
            case 'q': case 'Q': move.promotionPiece = PieceType::QUEEN; break;
            case 'r': case 'R': move.promotionPiece = PieceType::ROOK; break;
            case 'b': case 'B': move.promotionPiece = PieceType::BISHOP; break;
            case 'n': case 'N': move.promotionPiece = PieceType::KNIGHT; break;
            default: move.promotionPiece = PieceType::QUEEN; break;
        }
        move.type = MoveType::PROMOTION;
    }
    
    return move;
}

int AIPlayer::evaluate(const Board& board) const {
    int score = 0;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* piece = board.getPiece(Position(i, j));
            if (piece) {
                int pieceValue = piece->getValue();
                if (piece->getColor() == color) {
                    score += pieceValue;
                } else {
                    score -= pieceValue;
                }
            }
        }
    }
    
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    if (board.isCheckmate(opponent)) {
        return 1000000;
    }
    if (board.isCheckmate(color)) {
        return -1000000;
    }
    if (board.isStalemate(color) || board.isStalemate(opponent)) {
        return 0;
    }
    
    return score;
}

int AIPlayer::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) const {
    if (depth == 0) {
        return evaluate(board);
    }
    
    Color currentPlayer = board.getCurrentPlayer();
    std::vector<Move> moves = board.getLegalMoves(currentPlayer);
    
    if (moves.empty()) {
        if (board.isInCheck(currentPlayer)) {
            return maximizing ? -1000000 : 1000000;
        } else {
            return 0;
        }
    }
    
    if (maximizing) {
        int maxEval = std::numeric_limits<int>::min();
        for (const auto& move : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(move);
            int eval = minimax(tempBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (const auto& move : moves) {
            Board tempBoard = board;
            tempBoard.makeMove(move);
            int eval = minimax(tempBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha) {
                break;
            }
        }
        return minEval;
    }
}

Move AIPlayer::getMove(const Board& board) {
    std::cout << (color == Color::WHITE ? "White AI" : "Black AI") << " thinking (depth " << depth << ")...\n";
    
    std::vector<Move> legalMoves = board.getLegalMoves(color);
    
    if (legalMoves.empty()) {
        throw std::runtime_error("No legal moves available");
    }
    
    Move bestMove = legalMoves[0];
    int bestScore = (color == Color::WHITE) ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
    
    int moveCount = 0;
    for (const auto& move : legalMoves) {
        moveCount++;
        std::cout << "Evaluating move " << moveCount << "/" << legalMoves.size() << "...\r" << std::flush;
        
        Board tempBoard = board;
        tempBoard.makeMove(move);
        
        int score = minimax(tempBoard, depth - 1, 
                           std::numeric_limits<int>::min(), 
                           std::numeric_limits<int>::max(), 
                           color == Color::BLACK);
        
        if ((color == Color::WHITE && score > bestScore) || 
            (color == Color::BLACK && score < bestScore)) {
            bestScore = score;
            bestMove = move;
        }
    }
    
    std::cout << "\nMove selected! (Score: " << bestScore << ")\n";
    return bestMove;
} 