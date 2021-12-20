#pragma once

#include "agent.h"
#include "arms.h"
#include "bandits.h"
#include <map>

namespace multiArmedBandit {

class Logger
{
public:
    Logger();
    virtual ~Logger() = default;
    virtual void logActualArmsStats(const std::vector<ArmPtr>& arms);
    virtual void logActualAgentState(std::shared_ptr<IAgent> agent);
    void increaceIteration();
    virtual std::string printAll();
    virtual std::string printCacheStats();


protected:
    // Struct for internal use
    struct AgentStatsPerIteration
    {
        std::string name;
        double cache = 0;
        std::vector<double> armsCoeffs;
    };
    using agentsVector = std::vector<AgentStatsPerIteration>;

    size_t iteration = 0;
    std::vector<double> actualArmsStats;
    std::vector<agentsVector> agentStatsPerIteration;
};
} // namespace multiArmedBandit
