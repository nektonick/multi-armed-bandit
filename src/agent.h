#pragma once

#include <memory> // shared_ptr
#include <sstream>
#include <vector>
#include "bandits.h"
#include "strategies.h"

namespace multiArmedBandit {

using StrategyPtr = std::shared_ptr<IStrategy>;

class IAgent
{
public:
    IAgent(StrategyPtr strategy, double startCash);
    virtual ~IAgent() = default;

    virtual void runSingleRound(std::shared_ptr<IBandit> bandit);
    virtual std::string printStrategyName() const = 0;
    virtual double getCash() const;
    virtual std::vector<double> getArmsExpectation() const = 0;

protected:
    StrategyPtr strategy_;
    double cash_;
};

class SimpleAgent : public IAgent
{
public:
    SimpleAgent(StrategyPtr strategie, double startCache = 100);

    void runSingleRound(std::shared_ptr<IBandit> bandit) override;

    std::string printStrategyName() const override;
    std::vector<double> getArmsExpectation() const override;

protected:
};

} // namespace multiArmedBandit
