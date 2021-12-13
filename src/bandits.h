#pragma once

#include "arms.h"
#include <memory> // shared_ptr

namespace multiArmedBandit {

using ArmPtr = std::shared_ptr<IArm>;

/// Multi-armed bandit interface
class IBandit
{
public:
    IBandit();

    /// Return reward for chosen arm and increase iteration value;
    virtual double pullArmAndGetReward(size_t armIndex) = 0;

    /// Print number of iterations and and actual reward chance for each arm of
    /// this iteration
    virtual void statePrint();
};

class SimpleBandit : public IBandit
{
public:
    SimpleBandit(const std::vector<ArmPtr>& arms);

    void statePrint() override;

    double pullArmAndGetReward(size_t armIndex) override;

protected:
    std::vector<ArmPtr> arms_;
    size_t bestArmIndex;
    /// Turn or iteration number
    uint64_t iteration = 0;
};

} // namespace multiArmedBandit
