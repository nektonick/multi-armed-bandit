#include "stdafx.h"

#include "bandits.h"

namespace multiArmedBandit {

IBandit::IBandit() {}

SimpleBandit::SimpleBandit(const std::vector<ArmPtr>& arms, double pullCost)
    : arms_(arms)
    , pullCost_(pullCost)
{
    std::vector<double> expectedRewards(arms.size(), 0.0);
    for(size_t i = 0; i < arms_.size(); ++i)
    {
        expectedRewards[i] = arms_[i]->getRewardExpectation();
    }
    bestArmIndex_ = std::distance(expectedRewards.begin(), std::max_element(expectedRewards.begin(), expectedRewards.end()));
    ;
}

std::string SimpleBandit::statePrint()
{
    std::string out;
    for(size_t i = 0; i < arms_.size(); ++i)
    {
        out += arms_[i]->toString() + ENDL;
    }
    return out;
}

double SimpleBandit::pullArmAndGetReward(size_t armIndex)
{
    // Information to log: armIndex reward, regret
    // double bestRewardExpectation = arms_[bestArmIndex]->getRewardExpectation();
    // double currentRewardExpectation = arms_[armIndex]->getRewardExpectation();
    // double regret = bestRewardExpectation - currentRewardExpectation;
    double reward = arms_[armIndex]->pull();
    return reward;
}

double SimpleBandit::getPullCost() const
{
    return pullCost_;
}

size_t SimpleBandit::getArmsCount() const
{
    return arms_.size();
}

} // namespace multiArmedBandit
