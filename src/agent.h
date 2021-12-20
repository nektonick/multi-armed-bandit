#pragma once

#include "bandits.h"
#include "strategies.h"
#include <memory> // shared_ptr
#include <sstream>
#include <vector>

namespace multiArmedBandit {

using StrategyPtr = std::shared_ptr<IStrategy>;

class IAgent
{
public:
    IAgent(StrategyPtr strategy, double startCache);
    virtual ~IAgent() = default;

    virtual void runSingleRound(std::shared_ptr<IBandit> bandit);
    virtual std::string printStrategyName() const = 0;
    virtual double getCache() const;
    virtual std::vector<double> getArmsExpectation() const = 0;

protected:
    StrategyPtr strategy_;
    double cache_;
};

class AdvancedAgent : public IAgent
{
public:
    AdvancedAgent(StrategyPtr strategie, double startCache = 100);

    void runSingleRound(std::shared_ptr<IBandit> bandit) override;

    std::string printStrategyName() const override;
    std::vector<double> getArmsExpectation() const override;

protected:
};

} // namespace multiArmedBandit
