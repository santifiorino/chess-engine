#ifndef BOTS_H
#define BOTS_H

#include <memory>
#include "../Utils.h"
#include "../Position/Position.h"
#include "../MoveGenerator/MoveGenerator.h"

class RandomBot {
    public:
        RandomBot();
        Move makeMove(Position &position, MoveGenerator &moveGenerator);
    private:
        std::unique_ptr<RandomNumberGenerator> rng;
};

class MaterialDepthOneBot {
    public:
        MaterialDepthOneBot();
        Move makeMove(Position &position, MoveGenerator &moveGenerator);
    private:
        std::unique_ptr<RandomNumberGenerator> rng;
};

#endif