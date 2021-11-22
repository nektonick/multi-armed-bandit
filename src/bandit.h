#pragma once

#include "arm.h"
#include <memory> // std::unique_ptr
#include <vector>

namespace multiArmedBandit {

using ArmPtr = std::shared_ptr<IArm>;

/// Multi-armed bandit interface
/// This is automat with N arms
class IBandit{
public:
    IBandit(unsigned int armsCount);

    /// Return reward for chosen arm and increase iteration value;
    double makeChoice(uint64_t armNumber);

    /// Print number of iterations and and actual reward chance for each arm of this iteration
    virtual void statePrint();

protected:
    std::vector<ArmPtr> arms;
    /// Turn or iteration number
    uint64_t iteration = 0;
};

}
