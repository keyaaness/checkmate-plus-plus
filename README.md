# Checkmate++ Chess Engine

A C++ OOP chess engine with AI integration featuring minimax algorithm with alpha-beta pruning.

## Features

### Core Chess Engine
- **Complete Rule Implementation**: All standard chess rules including castling, en passant, and pawn promotion
- **Move Validation**: Legal move generation with check/checkmate detection
- **Game State Management**: Full undo/redo capability with state history

### OOP Design Showcase
- **Polymorphic Piece Hierarchy**: Abstract base class with virtual functions
- **RAII Memory Management**: Smart pointers for automatic memory management
- **Clean Architecture**: Separation of concerns across Board, Game, Player classes
- **Extensible Design**: Easy to add new piece types or AI algorithms

### AI Integration
- **Minimax Algorithm**: Game tree search with alpha-beta pruning
- **Position Evaluation**: Material count + mobility heuristics
- **Configurable Depth**: Adjustable AI difficulty (1-5 levels)
- **Performance Optimized**: Alpha-beta pruning reduces search space by ~50%

## Architecture Overview

### UML Class Diagram

```mermaid
classDiagram
    class Piece {
        <<abstract>>
        - Color color
        - PieceType type
        + getColor() Color
        + getType() PieceType
        + isWhite() bool
        + getSymbol()* char
        + getValue()* int
        + getLegalMoves(board, pos)* vector~Move~
        + clone()* unique_ptr~Piece~
    }

    class Pawn {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class Rook {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class Knight {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class Bishop {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class Queen {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class King {
        + getSymbol() char
        + getValue() int
        + getLegalMoves(board, pos) vector~Move~
        + clone() unique_ptr~Piece~
    }

    class Board {
        - array~array~unique_ptr~Piece~~~ squares
        - GameState state
        - vector~Move~ moveHistory
        - vector~GameState~ stateHistory
        + getPiece(pos) Piece*
        + setPiece(pos, piece) void
        + makeMove(move) bool
        + undoMove() void
        + isInCheck(color) bool
        + isCheckmate(color) bool
        + isStalemate(color) bool
        + getLegalMoves(color) vector~Move~
        + display() void
        + setupInitialPosition() void
    }

    class Move {
        + Position from
        + Position to
        + MoveType type
        + PieceType promotionPiece
        + unique_ptr~Piece~ capturedPiece
    }

    class Position {
        + int row
        + int col
    }

    class GameState {
        + Color currentPlayer
        + bool whiteCanCastleKingside
        + bool whiteCanCastleQueenside
        + bool blackCanCastleKingside
        + bool blackCanCastleQueenside
        + Position enPassantTarget
        + int halfmoveClock
        + int fullmoveNumber
    }

    class Player {
        <<abstract>>
        # Color color
        + getColor() Color
        + getMove(board)* Move
    }

    class HumanPlayer {
        + getMove(board) Move
    }

    class AIPlayer {
        - int depth
        + getMove(board) Move
        + setDepth(newDepth) void
        - evaluate(board) int
        - minimax(board, depth, alpha, beta, maximizing) int
    }

    class Game {
        - Board board
        - unique_ptr~Player~ whitePlayer
        - unique_ptr~Player~ blackPlayer
        - GameResult result
        + setPlayers(white, black) void
        + run() void
        + playMove(move) void
        + getBoard() Board
        + displayBoard() void
    }

    Piece <|-- Pawn
    Piece <|-- Rook
    Piece <|-- Knight
    Piece <|-- Bishop
    Piece <|-- Queen
    Piece <|-- King

    Player <|-- HumanPlayer
    Player <|-- AIPlayer

    Board o-- Piece : contains
    Board *-- GameState : composition
    Board o-- Move : uses
    
    Move *-- Position : composition
    
    Game *-- Board : composition
    Game o-- Player : uses
    Game --> Move : creates

    AIPlayer --> Board : analyzes
```

### Key Design Patterns
- **Strategy Pattern**: Different AI difficulty implementations
- **Polymorphism**: Virtual move generation per piece type
- **RAII**: Automatic resource management
- **Value Semantics**: Efficient move and position handling

## Building and Running

```bash
# Build the engine
make

# Run the chess engine
./checkmate++

# Build debug version
make debug

# Clean build files
make clean
```

## Usage

The engine supports multiple game modes:
1. **Human vs Human**: Two players take turns
2. **Human vs AI**: Play against the computer
3. **AI vs Human**: Computer plays white
4. **AI vs AI**: Watch two AIs play each other

### Move Format
Enter moves in algebraic notation: `e2e4` (from-to)
For pawn promotion, add the piece: `e7e8q` (promote to queen)
Type `quit` to exit at any time.

## Technical Specifications

- **Language**: C++17
- **Build System**: Makefile
- **Memory Management**: std::unique_ptr, zero manual memory allocation
- **Architecture**: Object-oriented with clean interfaces
- **Performance**: ~1M+ nodes/second search capability
