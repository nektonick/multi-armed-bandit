#include "stdafx.h"

#include "agent.h"

namespace multiArmedBandit {

IAgent::IAgent(StrategyPtr strategy, double startCash)
    : strategy_(std::move(strategy))
    , cash_(startCash)
{}

void IAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    double reward = bandit->pullArmAndGetReward(armToPush);
    strategy_->updateState(armToPush, reward);
}

double IAgent::getCash() const
{
    return cash_;
}

SimpleAgent::SimpleAgent(StrategyPtr strategie, double startCache)
    : IAgent(std::move(strategie), startCache)
{}

void SimpleAgent::runSingleRound(std::shared_ptr<IBandit> bandit)
{
    size_t armToPush = strategy_->selectNextArm();
    cash_ -= bandit->getPullCost();
    double reward = bandit->pullArmAndGetReward(armToPush);
    cash_ += reward;
    strategy_->updateState(armToPush, reward);
}

std::string SimpleAgent::printStrategyName() const
{
    std::stringstream ss;
    ss << strategy_->getName();
    return ss.str();
}

std::vector<double> SimpleAgent::getArmsExpectation() const
{
    return strategy_->getArmsExpectation();
}

} // namespace multiArmedBandit
