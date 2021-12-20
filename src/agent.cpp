#include "stdafx.h"

#include "agent.h"

namespace multiArmedBandit {

IAgent::IAgent(StrategyPtr strategy, double startCache)
    : strategy_(std::move(strategy))
    , cache_(startCache)
{}

void IAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    double reward = bandit->pullArmAndGetReward(armToPush);
    strategy_->updateState(armToPush, reward);
}

double IAgent::getCache() const
{
    return cache_;
}

AdvancedAgent::AdvancedAgent(StrategyPtr strategie, double startCache)
    : IAgent(std::move(strategie), startCache)
{}

void AdvancedAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    cache_ -= bandit->getPullCost();
    double reward = bandit->pullArmAndGetReward(armToPush);
    cache_ += reward;
    strategy_->updateState(armToPush, reward);
}

std::string AdvancedAgent::printStrategyName() const
{
    std::stringstream ss;
    ss << strategy_->getName();
    return ss.str();
}

std::vector<double> AdvancedAgent::getArmsExpectation() const
{
    return strategy_->getArmsExpectation();
}

} // namespace multiArmedBandit
