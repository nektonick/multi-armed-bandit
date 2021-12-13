#include "stdafx.h"

#include "agent.h"

namespace multiArmedBandit {

IAgent::IAgent(StrategyPtr strategy)
    : strategy_(std::move(strategy))
{}

void IAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    double reward = bandit->pullArmAndGetReward(armToPush);
    strategy_->updateState(armToPush, reward);
}

AdvancedAgent::AdvancedAgent(StrategyPtr strategie, double startCache)
    : IAgent(std::move(strategie))
    , cache_(startCache)
{}

void AdvancedAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    cache_ -= bandit->getPullCost();
    double reward = bandit->pullArmAndGetReward(armToPush);
    cache_ += reward;
    strategy_->updateState(armToPush, reward);
}

std::string AdvancedAgent::printInfo() const
{
    std::stringstream ss;
    ss << "Agent with strategy " << strategy_->toString() << " with cache = " << std::fixed << cache_;
    return ss.str();
}

} // namespace multiArmedBandit
