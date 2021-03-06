#pragma once

#include <memory> // shared_ptr
#include "arms.h"

namespace multiArmedBandit {

using ArmPtr = std::shared_ptr<IArm>;

/// Multi-armed bandit interface
class IBandit
{
public:
    IBandit();
    virtual ~IBandit() = default;
    /// Return reward for chosen arm and increase iteration value;
    virtual double pullArmAndGetReward(size_t armIndex) = 0;

    virtual double getPullCost() const = 0;

    virtual size_t getArmsCount() const = 0;

    /// Print number of iterations and and actual reward chance for each arm of
    /// this iteration
    virtual std::string statePrint() = 0;
};

class SimpleBandit : public IBandit
{
public:
    SimpleBandit(const std::vector<ArmPtr>& arms, double pullCost_ = 0);

    std::string statePrint() override;

    double pullArmAndGetReward(size_t armIndex) override;
    double getPullCost() const override;
    virtual size_t getArmsCount() const override;

protected:
    std::vector<ArmPtr> arms_;
    size_t bestArmIndex_;
    const double pullCost_;
};

} // namespace multiArmedBandit
