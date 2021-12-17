#include "stdafx.h"

#include "agent.h"

using namespace multiArmedBandit;

int main()
{
    const size_t armsCount = 300;
    const size_t iterationsCount = 5000;

    std::vector<ArmPtr> arms;
    for(size_t i = 0; i < armsCount; ++i)
    {
        auto arm = std::make_shared<BernoulliArm>();
        arms.push_back(arm);
    }
    std::shared_ptr<SimpleBandit> bandit = std::make_shared<SimpleBandit>(arms);

    AdvancedAgent agentRandom(std::make_shared<RandomStrategy>(bandit->getArmsCount()));
    AdvancedAgent agentDMED(std::make_shared<DMEDBinaryStrategy>(bandit->getArmsCount()));

    std::cout << "The initial agents state:" ENDL;
    std::cout << agentRandom.printInfo() << ENDL;
    std::cout << agentDMED.printInfo() << ENDL;
    std::cout << ENDL "Start of test" ENDL;

    std::cout << '[';
    for(size_t iteration = 0; iteration < iterationsCount; ++iteration)
    {
        agentRandom.runSingleRound(bandit);
        agentDMED.runSingleRound(bandit);
        if(iteration % (iterationsCount / 100) == 0)
        {
            std::cout << '=';
            // we force to output the buffer
            std::flush(std::cout);
        }
    }
    std::cout << ']';
    std::cout << ENDL "End of test" ENDL;
    std::cout << ENDL "The final agents state:" ENDL;
    std::cout << agentRandom.printInfo() << ENDL;
    std::cout << agentDMED.printInfo() << ENDL;
    return 0;
}
