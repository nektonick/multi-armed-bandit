#pragma once

#include "common_utils.h"

namespace multiArmedBandit {

/// Arm interface with virtual functions
class IArm
{
public:
    /// @note Default base constructor will be called in all child classes constructors
    IArm();
    virtual ~IArm() = default;
    virtual void setRandomeSeed(uint8_t seed) final;

    /// Pull this arm and get reward
    virtual double pull() = 0;

    /// Get actual arm reward characteristic (chance or mean value)
    virtual double getRewardExpectation() = 0;

    virtual std::string toString() = 0;

protected:
    std::mt19937 randomEngine;
};

/// Binary (win or lose) arm (a.k.a. Bernoulli arm)
class BernoulliArm : public IArm
{
public:
    BernoulliArm(double rewardMultiplier = 1);
    BernoulliArm(double rewardChance, double rewardMultiplier);

    double pull() override;
    double getRewardExpectation() override;

    std::string toString() override;

private:
    double rewardValueMult_;
    std::uniform_real_distribution<double> realDistr_;
    const double rewardChance_;
};

/// Arm that always returns some reward from an interval
class NormalArm : public IArm
{
public:
    NormalArm(double rewardMin, double rewardMax);

    double pull() override;

    double getRewardExpectation() override;

    std::string toString() override;

protected:
    const double rewardMin_;
    const double rewardMax_;
    std::normal_distribution<double> normalDistribution_;
};

} // namespace multiArmedBandit
