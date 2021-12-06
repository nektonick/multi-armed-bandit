#pragma once

#include "common_utils.h"
#include <cmath>
#include <string>

namespace multiArmedBandit {

/// Arm interface with virtual functions
class IArm
{
public:
    // pull this arm and get reward
    virtual double pull() = 0;

public:
    virtual std::string toString() = 0;
};

// Binary (win or lose) arm (a.k.a. Bernoulli arm)
class BernoulliArm : public IArm
{
public:
    BernoulliArm();

    double pull() override;

    virtual double getActualWinChance();

    std::string toString() override;

private:
    double rewardValueMult;
    const double rewardChance;
};

} // namespace multiArmedBandit
