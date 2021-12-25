#include "Logger.h"

namespace multiArmedBandit {
Logger::Logger() {}

void Logger::logActualArmsStats(const std::vector<ArmPtr>& arms)
{
    actualArmsStats.reserve(arms.size());
    for(const auto& arm : arms)
    {
        actualArmsStats.push_back(arm->getRewardExpectation());
    }
    agentStatsPerIteration.push_back(agentsVector{});
}

void Logger::logActualAgentState(std::shared_ptr<IAgent> agent)
{
    AgentStatsPerIteration agentStats;
    agentStats.name = agent->printStrategyName();
    agentStats.cache = agent->getCash();
    agentStats.armsCoeffs = agent->getArmsExpectation();

    agentStatsPerIteration[iteration].push_back(std::move(agentStats));
}

std::string Logger::printAll()
{
    std::ostringstream out;
    out << "Actual arms reward chance: " ENDL;
    {
        size_t armIndex = 0;
        for(const auto armChance : actualArmsStats)
        {
            out << "Arm#" << armIndex << ": " << armChance << ENDL;
            ++armIndex;
        }
    }

    out << ENDL;

    out << "Agents stats per iterations: " ENDL;
    size_t iterationNum = 0;

    for(const auto& iteration : agentStatsPerIteration)
    {
        out << "Iteration#" << iterationNum << ":" ENDL;
        for(const auto& agent : iteration)
        {
            out << TAB << "Agent: " << agent.name << ENDL;
            out << TAB << "Cache: " << agent.cache << ENDL;
            out << TAB << "Expectations: " ENDL;
            size_t armIndex = 0;
            for(const auto coeff : agent.armsCoeffs)
            {
                out << TAB << TAB << "Arm#" << armIndex << ": " << coeff << ENDL;
                ++armIndex;
            }
            out << TAB << std::string(26, '_') << ENDL;
        }
        out << std::string(30, '_') << ENDL;
        ++iterationNum;
    }

    return out.str();
}

std::string Logger::printCacheStats()
{
    std::ostringstream out;
    out << "Actual arms reward chance: " ENDL;
    {
        size_t armIndex = 0;
        for(const auto armChance : actualArmsStats)
        {
            out << "Arm#" << armIndex << ": " << armChance << ENDL;
            ++armIndex;
        }
    }

    out << ENDL;

    out << "Cache stats per iterations: " ENDL;
    size_t iterationNum = 0;

    for(const auto& iteration : agentStatsPerIteration)
    {
        out << "Iteration#" << iterationNum << ":" ENDL;
        for(const auto& agent : iteration)
        {
            out << TAB << agent.name << ": " << agent.cache << "$" ENDL;
        }
        out << std::string(30, '_') << ENDL;
        ++iterationNum;
    }

    return out.str();
}

void Logger::increaceIteration()
{
    agentStatsPerIteration.push_back(agentsVector{});
    ++iteration;
}
} // namespace multiArmedBandit
