//
// Created by Kutlay Acar on 22.02.22.
//

#include "replay.h"
#include <iostream>

// Asks player if she/he wants to play again
void Replay::gameContinue() {

    std::string gContinue;
    std::cout << "Do you want to play again? y/n \n";

//    std::cin >> gContinue;
//    if (gContinue == "n" || gContinue == "N") _again = false;

    while (std::cin >> gContinue){

        if (gContinue == "y" || gContinue == "Y") break;
        else if (gContinue == "n" || gContinue == "N"){
            _again = false;
            std::cout << "Goodbye!\n";
            break;
        }
        else{
            std::cerr << "WRONG INPUT!" << " Do you want to play again? y/n \n";
        }
    }
}

bool Replay::playAgain() const { return _again;}
