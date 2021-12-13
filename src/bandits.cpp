#include "stdafx.h"

#include "bandits.h"

namespace multiArmedBandit {

SimpleBandit::SimpleBandit(const std::vector<ArmPtr>& arms)
    : arms_(arms)
{
    std::vector<double> expectedRewards(arms.size(), 0.0);
    for(size_t i = 0; i < arms_.size(); ++i)
    {
        expectedRewards[i] = arms_[i]->getRewardExpectation();
    }
    bestArmIndex = std::distance(expectedRewards.begin(), std::max_element(expectedRewards.begin(), expectedRewards.end()));
    ;
}

void SimpleBandit::statePrint()
{
    for(size_t i = 0; i < arms_.size(); ++i)
    {
        std::cout << arms_[i]->toString() << ENDL;
    }
}

double SimpleBandit::pullArmAndGetReward(size_t armIndex)
{
    //uint armIndex = policies[p]->selectNextArm();
    double bestRewardChance = arms_[bestArmIndex]->getRewardExpectation();
    double currentRewardChance = arms_[armIndex]->getRewardExpectation();
    double regret = bestRewardChance - currentRewardChance;
    double reward = arms_[armIndex]->pull();
    //policies[p]->updateState(armIndex, reward);
    //log.record(p, t, armIndex, reward, regret);
    return reward;
}

} // namespace multiArmedBandit
