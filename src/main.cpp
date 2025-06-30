#include "game.hpp"
#include "player.hpp"
#include <iostream>
#include <memory>

void displayMenu() {
    std::cout << "\n~~~~~~ Checkmate++ Chess Engine ~~~~~~\n";
    std::cout << "1. Human vs Human\n";
    std::cout << "2. Human vs AI\n";
    std::cout << "3. AI vs Human\n";
    std::cout << "4. AI vs AI\n";
    std::cout << "5. Exit\n";
    std::cout << "Choose an option: ";
}

int getAIDifficulty() {
    int difficulty;
    std::cout << "Select AI difficulty (1-5): ";
    std::cin >> difficulty;
    std::cin.ignore();
    return std::max(1, std::min(5, difficulty));
}

int main() {
    int choice;
    
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 5) {
            std::cout << "Thanks for playing!\n";
            break;
        }
        
        Game game;
        std::unique_ptr<Player> whitePlayer;
        std::unique_ptr<Player> blackPlayer;
        
        switch (choice) {
            case 1:
                whitePlayer = std::make_unique<HumanPlayer>(Color::WHITE);
                blackPlayer = std::make_unique<HumanPlayer>(Color::BLACK);
                break;
                
            case 2: {
                int difficulty = getAIDifficulty();
                whitePlayer = std::make_unique<HumanPlayer>(Color::WHITE);
                blackPlayer = std::make_unique<AIPlayer>(Color::BLACK, difficulty);
                break;
            }
            
            case 3: {
                int difficulty = getAIDifficulty();
                whitePlayer = std::make_unique<AIPlayer>(Color::WHITE, difficulty);
                blackPlayer = std::make_unique<HumanPlayer>(Color::BLACK);
                break;
            }
            
            case 4: {
                std::cout << "White AI difficulty: ";
                int whiteDifficulty = getAIDifficulty();
                std::cout << "Black AI difficulty: ";
                int blackDifficulty = getAIDifficulty();
                whitePlayer = std::make_unique<AIPlayer>(Color::WHITE, whiteDifficulty);
                blackPlayer = std::make_unique<AIPlayer>(Color::BLACK, blackDifficulty);
                break;
            }
            
            default:
                std::cout << "Invalid choice. Please try again.\n";
                continue;
        }
        
        game.setPlayers(std::move(whitePlayer), std::move(blackPlayer));
        game.run();
    }
    
    return 0;
} 