#pragma once
#include "board.hpp"
#include "player.hpp"
#include <memory>

enum class GameResult { ONGOING, WHITE_WINS, BLACK_WINS, DRAW };

class Game {
private:
    Board board;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    GameResult result;
    
    bool parseMove(const std::string& moveStr, Move& move) const;
    std::string moveToString(const Move& move) const;
    Position parsePosition(const std::string& posStr) const;
    std::string positionToString(const Position& pos) const;
    
public:
    Game();
    
    void setPlayers(std::unique_ptr<Player> white, std::unique_ptr<Player> black);
    void run();
    void playMove(const Move& move);
    
    const Board& getBoard() const { return board; }
    GameResult getResult() const { return result; }
    
    void displayBoard() const;
    void displayGameInfo() const;
}; 