#pragma once
#include "board.hpp"
#include "move.hpp"
#include <algorithm>

class Player {
protected:
    Color color;
    
public:
    Player(Color c) : color(c) {}
    virtual ~Player() = default;
    
    virtual Move getMove(const Board& board) = 0;
    Color getColor() const { return color; }
};

class HumanPlayer : public Player {
public:
    HumanPlayer(Color c) : Player(c) {}
    Move getMove(const Board& board) override;
};

class AIPlayer : public Player {
private:
    int depth;
    
    int evaluate(const Board& board) const;
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizing) const;
    
public:
    AIPlayer(Color c, int searchDepth = 3) : Player(c), depth(std::min(searchDepth, 3)) {}
    Move getMove(const Board& board) override;
    
    void setDepth(int newDepth) { depth = newDepth; }
    int getDepth() const { return depth; }
}; 