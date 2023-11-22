//
// Created by Kutlay Acar on 22.02.22.
//

#include "highscore.h"
// Reads the lines from highscore.txt
void HighScore::Read(void) {

    std::string line, name, score;
    Player player;
    std::ifstream stream("../highscore.txt");
    if (stream.is_open()){
        while (std::getline(stream, line)){
            std::istringstream linestream(line);
            while (linestream >> name >> score){
                player.setName(name);
                player.setScore(std::stoi(score));
                _playerList.emplace_back(player);
            }
        }
    }
}
// Write new highscores in
void HighScore::Write(void) {

    std::ofstream file;
    file.open ("../highscore.txt");
    for (std::vector<Player>::iterator iter = _playerList.begin(); iter != _playerList.end(); ++iter){
        file << iter->getName() << " " << iter->getScore() << "\n";
    }
    file.close();
}

// After finishing the game print first 5 highscores
void HighScore::Print(void) {
    int i = 0;
    std::cout << "HIGHSCORES:  \n";
    std::cout << "-----------\n";
    for (std::vector<Player>::iterator iter = _playerList.begin(); iter < _playerList.begin() + 5; ++iter) {
        i++;
        std::cout << "Rank " << i << ":\t" << iter->getName() << "\t" << iter->getScore() << "\n";
    }
    std::cout << "\n";
}

// Update highscore.txt
int HighScore::Update(std::string playerName, int score) {
    Player player;
    player.setName(playerName);
    player.setScore(score);

    // put new score in highscore.txt and sort the list
    if (_playerList.back().getScore() < score){
        _playerList.emplace_back(player);
        std::sort(_playerList.begin(), _playerList.end(), Compare);
    }
    return score;
}
// Compare algorithm for sorting the list
bool Compare(Player p1, Player p2){
    return p1.getScore() > p2.getScore();
}