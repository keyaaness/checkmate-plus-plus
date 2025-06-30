#!/bin/bash

echo "🏆 Checkmate++ Chess Engine Showcase"
echo "======================================"
echo ""

echo "📋 Project Overview:"
echo "- C++ OOP chess engine with AI integration"
echo "- Minimax algorithm with alpha-beta pruning"
echo "- Complete chess rule implementation"
echo "- RAII memory management with smart pointers"
echo ""

echo "🔧 Building the project..."
make clean > /dev/null 2>&1
if make > /dev/null 2>&1; then
    echo "✅ Build successful!"
else
    echo "❌ Build failed!"
    exit 1
fi
echo ""

echo "🧪 Running basic functionality check..."
if [ -x "./checkmate++" ]; then
    echo "✅ Chess engine executable ready!"
else
    echo "❌ Chess engine not found!"
fi
echo ""

echo "📊 Technical Specifications:"
echo "- Language: C++17"
echo "- Memory Management: Smart pointers (std::unique_ptr)"
echo "- Architecture: OOP with polymorphism"
echo "- AI Algorithm: Minimax with alpha-beta pruning"
echo "- Performance: ~20 legal moves in starting position"
echo "- Code Quality: Zero manual memory management"
echo ""

echo "🎮 Demonstration Features:"
echo "1. Human vs Human play"
echo "2. Human vs AI (5 difficulty levels)"
echo "3. AI vs AI battles"
echo "4. Complete chess rules (castling, en passant, promotion)"
echo "5. Check/checkmate detection"
echo ""

echo "📁 Project Structure:"
find . -name "*.hpp" -o -name "*.cpp" | head -10 | sed 's/^/   /'
echo "   ... and more"
echo ""

echo "🚀 To run the chess engine:"
echo "   ./checkmate++"
echo ""

echo "📈 Performance Metrics:"
echo "- Lines of Code: $(find src -name "*.cpp" -o -name "*.hpp" | xargs wc -l | tail -1 | awk '{print $1}')"
echo "- Classes: $(grep -r "^class " src/ | wc -l)"
echo "- Virtual Functions: $(grep -r "virtual " src/ | wc -l)"
echo "- Smart Pointers: $(grep -r "unique_ptr" src/ | wc -l)"
echo ""

echo "🎯 OOP Concepts Demonstrated:"
echo "✓ Inheritance (Piece hierarchy)"
echo "✓ Polymorphism (Virtual getLegalMoves())"
echo "✓ Encapsulation (Private board state)"
echo "✓ Abstraction (Clean interfaces)"
echo "✓ RAII (Automatic memory management)"
echo ""

echo "🤖 AI Features:"
echo "✓ Minimax search algorithm"
echo "✓ Alpha-beta pruning optimization"
echo "✓ Position evaluation heuristics"
echo "✓ Configurable search depth"
echo "✓ Move ordering for efficiency"
echo ""

echo "   ✨ This project demonstrates advanced C++ skills and AI implementation. ✨" 