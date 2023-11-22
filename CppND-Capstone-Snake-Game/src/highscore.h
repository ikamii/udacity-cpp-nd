//
// Created by Kutlay Acar on 22.02.22.
//

#ifndef SDL2TEST_HIGHSCORE_H
#define SDL2TEST_HIGHSCORE_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "player.h"

const int kMaxentries = 100;

class HighScore{
public:
    void Read(void);
    void Write(void);
    void Print(void);
    int Update(std::string playerName, int score);

private:
    std::vector<Player> _playerList;

};

bool Compare(Player p1, Player p2);


#endif //SDL2TEST_HIGHSCORE_H


