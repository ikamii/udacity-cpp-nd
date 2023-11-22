//
// Created by Kutlay Acar on 22.02.22.
//

#ifndef SDL2TEST_REPLAY_H
#define SDL2TEST_REPLAY_H

#include <string>

class Replay {
public:
    void gameContinue();
    bool playAgain() const;

private:
    bool _again = true;
};

#endif //SDL2TEST_REPLAY_H
