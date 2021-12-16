#include "stdafx.h"

#include "strategies.h"

namespace multiArmedBandit {

IStrategy::IStrategy()
    : randomEngine(std::time(0))
{}

void IStrategy::setRandomSeed(uint64_t randomSeed)
{
    randomEngine.seed(randomSeed);
}

RandomStrategy::RandomStrategy(size_t armsCount)
    : armsCount_(armsCount)
{}

int RandomStrategy::selectNextArm()
{
    return std::uniform_int_distribution<int>(0, armsCount_ - 1)(randomEngine);
}

void RandomStrategy::updateState(int, double)
{
    // do nothing
}

std::string RandomStrategy::toString()
{
    std::string str = "Random";
    return str;
}

DMEDBinaryStrategy::DMEDBinaryStrategy(size_t armsCount)
    : armsCount_(armsCount)
    , epsilonValue_(0.0001)
    , iterationNumber_(0)
{
    for(size_t i = 0; i < armsCount; ++i)
    {
        armPullsCount.push_back(0);
        armTotalReward.push_back(0);
        armsToPull_.insert(i);
    }
    LRc.clear();
    LN.clear();
}

int DMEDBinaryStrategy::selectNextArm()
{
    for(size_t i = 0; i < armsCount_; ++i)
    {
        // Firstly pull all arms at least once
        if(armPullsCount[i] == 0)
        {
            return i;
        }
    }

    return (*armsToPull_.begin());
}

void DMEDBinaryStrategy::updateState(int selectedArmIndex, double reward)
{
    ++iterationNumber_;
    armPullsCount[selectedArmIndex] += 1;
    armTotalReward[selectedArmIndex] += reward;

    armsToPull_.erase(selectedArmIndex);

    if(iterationNumber_ <= armsCount_)
    {
        //initial exploration, so skip other logic and return
        return;
    }

    // Some strange math stuff. I have no idea, what's going on below
    // So, I can't improve variables naming

    std::vector<double> mus(armsCount_, 0.0);
    for(size_t i = 0; i < armsCount_; ++i)
    {
        mus[i] = std::max(std::min(armTotalReward[i] / armPullsCount[i], 1.0 - epsilonValue_), epsilonValue_);
    }

    LRc.insert(selectedArmIndex);
    const double mu_opt = *(std::max_element(mus.begin(), mus.end()));
    for(const auto& j : LRc)
    {
        const double dj = dualDivergence(mus[j], mu_opt);
        if(armPullsCount[j] * dj <= std::log((double)iterationNumber_ / armPullsCount[j]))
        {
            LN.insert(j);
        }
    }

    if(armsToPull_.empty())
    {
        armsToPull_ = LN;
        LRc.clear();
        for(size_t i = 0; i < armsCount_; ++i)
        {
            if(armsToPull_.find(i) == armsToPull_.end())
            {
                LRc.insert(i);
            }
        }
        LN.clear();
    }
}

std::string DMEDBinaryStrategy::toString()
{
    std::string str = "DMED (binary)";
    return str;
}

double DMEDBinaryStrategy::dualDivergence(double mu, double mu_opt)
{
    // Some strange math stuff. I have no idea, what's going on below
    // So, I can't improve variables naming

    double nu = (mu_opt - mu) / ((1 - mu_opt) * mu_opt);
    return std::log(1 + mu_opt * nu) * (1 - mu) + std::log(1 - (1 - mu_opt) * nu) * mu;
}

} // namespace multiArmedBandit
