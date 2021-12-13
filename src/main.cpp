#include "stdafx.h"

#include <iomanip>
#include "agent.h"

using namespace multiArmedBandit;

int main()
{
    const size_t armsCount = 10;
    const size_t iterationsCount = 300;

    std::vector<ArmPtr> arms;
    for(size_t i = 0; i < armsCount; ++i)
    {
        auto arm = std::make_shared<BernoulliArm>();
        arms.push_back(arm);
    }
    std::shared_ptr<SimpleBandit> bandit = std::make_shared<SimpleBandit>(arms);

    AdvancedAgent agentRandom(std::make_shared<RandomStrategy>(bandit->getArmsCount()));
    AdvancedAgent agentDMED(std::make_shared<DMEDBinaryStrategy>(bandit->getArmsCount()));

    std::cout << "Start of test" ENDL;
    for(size_t iteration = 0; iteration < iterationsCount; ++iteration)
    {
        std::cout << "Round#" << iteration << ENDL;
        agentRandom.runSingleRound(bandit);
        std::cout << agentRandom.printInfo() << ENDL;
        agentDMED.runSingleRound(bandit);
        std::cout << agentDMED.printInfo()<< ENDL;
    }
    std::cout << "End of test" ENDL;
    return 0;
}
