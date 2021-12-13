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
    IAgent(StrategyPtr strategy);

    virtual void runSingleRound(std::shared_ptr<IBandit> bandit);

    virtual std::string printInfo() const = 0;

protected:
    StrategyPtr strategy_;
};


class AdvancedAgent : public IAgent
{
public:
    AdvancedAgent(StrategyPtr strategie, double startCache = 100.0);

    void runSingleRound(std::shared_ptr<IBandit> bandit) override;

    std::string printInfo() const override;

protected:
    double cache_;
};

} // namespace multiArmedBandit
