#include "stdafx.h"

#include "arms.h"

namespace multiArmedBandit {

IArm::IArm()
{
    std::random_device realyRandomGenerator;
    uint32_t seed = realyRandomGenerator();
    randomEngine.seed(seed);
}

void IArm::setRandomeSeed(uint64_t seed)
{
    randomEngine.seed(seed);
}

BernoulliArm::BernoulliArm()
    : rewardValueMult_(1)
    , realDistr_(0, 1)
    , rewardChance_(realDistr_(randomEngine))
{}

BernoulliArm::BernoulliArm(double rewardChance)
    : rewardValueMult_(1)
    , realDistr_(0, 1)
    , rewardChance_(rewardChance)

{}

double BernoulliArm::pull()
{
    // generate random value within the realDistr interval
    double randomValue = realDistr_(randomEngine);

    // NOTE:
    // Line below is ternary operator:
    // if rewardChance >= randomValue reward will be returned; else zero will be returned
    // NOTE:
    // std::isgreaterequal() function is used instead of operator>=() to avoid FE_INVALID exception, however
    // there is no way rewardChance and randomValue become NaN, so using of isgreaterequal() is not really necessary
    return std::isgreaterequal(rewardChance_, randomValue) ? (1.0 * rewardValueMult_) : 0.0;
}

double BernoulliArm::getRewardExpectation()
{
    return rewardChance_;
}

std::string BernoulliArm::toString()
{
    return "BernoulliArm with reward chance=" + std::to_string(rewardChance_);
}

NormalArm::NormalArm(double rewardMin, double rewardMax)
    : rewardMin_(rewardMin)
    , rewardMax_(rewardMax)
    , normalDistribution_(rewardMin_, rewardMax_)
{}

double NormalArm::pull()
{
    return normalDistribution_(randomEngine);
}

double NormalArm::getRewardExpectation()
{
    return normalDistribution_.mean();
}

std::string NormalArm::toString()
{
    std::string str = "NormalArm with min reward=" + std::to_string(rewardMin_) + ", max reward=" + std::to_string(rewardMax_) +
                      " and mean reward=" + std::to_string(rewardMin_);
    return str;
}

} // namespace multiArmedBandit
