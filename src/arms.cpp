#include "arms.h"

multiArmedBandit::BernoulliArm::BernoulliArm()
    : IArm()
    , rewardValueMult(1.0)
    , rewardChance(realDistr(randomEngine))
{}

double multiArmedBandit::BernoulliArm::pull()
{
    // generate random value within the realDistr interval
    double randomValue = realDistr(randomEngine);

    // NOTE:
    // Line below is ternary operator:
    // if rewardChance >= randomValue reward will be returned; else zero will be returned
    // NOTE:
    // std::isgreaterequal() funtion is used instead of operator>=() to avoid FE_INVALID exception, however
    // there is no way rewardChance and randomValue become NaN, so using of isgreaterequal() is not really necessary
    return std::isgreaterequal(rewardChance, randomValue) ? (1.0 * rewardValueMult) : 0.0;
}

double multiArmedBandit::BernoulliArm::getActualWinChance()
{
    return rewardChance;
}

std::string multiArmedBandit::BernoulliArm::toString()
{
    return "Arm type: BernoulliArm;" ENDL "rewardChance: " + std::to_string(rewardChance) + ";";
}
