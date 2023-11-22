//
// Created by Kutlay Acar on 22.02.22.
//

#ifndef SDL2TEST_PLAYER_H
#define SDL2TEST_PLAYER_H

#include <string>

class Player {
public:
    void setName(std::string playerName);
    void setScore(int score);

    std::string getName() {return _playerName;}
    int getScore() {return _score;}


private:
    std::string _playerName;
    int _score;
};



#endif //SDL2TEST_PLAYER_H
