#include "stdafx.h"

#include "agent.h"
#include "logger.h"

using namespace multiArmedBandit;

int main()
{
    // TODO: get params from cmd arguments

    const size_t armsCount = 3;
    const size_t iterationsCount = 10000;
    const double pullCost = 0.5;
    const double rewardMultiplier = 1;

    Logger logger;
    enum class LogOptions
    {
        none = 0,
        cacheOnly,
        full
    };
    LogOptions logOptinos = LogOptions::cacheOnly;

    std::vector<ArmPtr> arms;
    for(size_t i = 0; i < armsCount; ++i)
    {
        auto arm = std::make_shared<BernoulliArm>(rewardMultiplier);
        arms.push_back(arm);
    }

    std::shared_ptr<SimpleBandit> bandit = std::make_shared<SimpleBandit>(arms, pullCost);

    std::vector<std::shared_ptr<SimpleAgent>> agents = {std::make_shared<SimpleAgent>(std::make_shared<RandomStrategy>(bandit->getArmsCount())),
                                                        std::make_shared<SimpleAgent>(std::make_shared<DMED_BinaryStrategy>(bandit->getArmsCount())),
                                                        std::make_shared<SimpleAgent>(std::make_shared<KL_UCBStrategy>(bandit->getArmsCount()))};

    logger.logActualArmsStats(arms);

    std::cout << "Start of test" ENDL;
    std::cout << '[';
    std::flush(std::cout);
    for(size_t iteration = 0; iteration < iterationsCount; ++iteration)
    {
        for(auto& agent : agents)
        {
            agent->runSingleRound(bandit);
            logger.logActualAgentState(agent);
        }
        logger.increaceIteration();

        // loading bar printing
        if(iterationsCount >= 100)
        {
            if(iteration % (iterationsCount / 100) == 0)
            {
                std::cout << '=';
            }
        }
        else
        {
            /// string of (100 / iterationsCount) symbols
            std::string loadingStringPart(100 / iterationsCount, '=');
            std::cout << loadingStringPart;
        }
        // force to output the buffer
        std::flush(std::cout);
    }
    std::cout << ']';
    std::cout << ENDL "End of test" ENDL;

    switch(logOptinos)
    {
    case LogOptions::cacheOnly:
        std::cout << ENDL << logger.printCacheStats();
        break;
    case LogOptions::full:
        std::cout << ENDL << logger.printAll();
        break;
    case LogOptions::none:
        // nothing (drop down to default)
    default:
        std::cout << "Output minimized";
        break;
    }

    return 0;
}
