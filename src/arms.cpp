#include "stdafx.h"

#include "arms.h"

multiArmedBandit::BernoulliArm::BernoulliArm()
    : IArm()
    , rewardValueMult_(1.0)
    , realDistr_(0.0, 1.0)
    , rewardChance_(realDistr_(randomEngine))
{}

multiArmedBandit::BernoulliArm::BernoulliArm(double rewardChance)
    : rewardChance_(rewardChance)
{}

double multiArmedBandit::BernoulliArm::pull()
{
    // generate random value within the realDistr interval
    double randomValue = realDistr_(randomEngine);

    // NOTE:
    // Line below is ternary operator:
    // if rewardChance >= randomValue reward will be returned; else zero will be returned
    // NOTE:
    // std::isgreaterequal() funtion is used instead of operator>=() to avoid FE_INVALID exception, however
    // there is no way rewardChance and randomValue become NaN, so using of isgreaterequal() is not really necessary
    return std::isgreaterequal(rewardChance_, randomValue) ? (1.0 * rewardValueMult_) : 0.0;
}

double multiArmedBandit::BernoulliArm::getRewardExpectation()
{
    return rewardChance_;
}

std::string multiArmedBandit::BernoulliArm::toString()
{
    return "BernoulliArm with reward chance=" + std::to_string(rewardChance_);
}

multiArmedBandit::NormalArm::NormalArm(double rewardMin, double rewardMax)
    : rewardMin_(rewardMin)
    , rewardMax_(rewardMax)
    , normalDistribution_(rewardMin_, rewardMax_)
{}

double multiArmedBandit::NormalArm::pull()
{
    return normalDistribution_(randomEngine);
}

double multiArmedBandit::NormalArm::getRewardExpectation()
{
    return normalDistribution_.mean();
}

std::string multiArmedBandit::NormalArm::toString()
{
    std::string str = "NormalArm with min reward=" + std::to_string(rewardMin_) + ", max reward=" + std::to_string(rewardMax_) +
                      " and mean reward=" + std::to_string(rewardMin_);
    return str;
}
