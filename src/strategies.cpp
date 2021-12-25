#include "stdafx.h"

#include "strategies.h"
#include <numeric> // std::accumulate

namespace multiArmedBandit {

RandomStrategy::RandomStrategy(size_t armsCount)
    : armsCount_(armsCount)
{
    std::random_device trueRandomGenerator;
    uint32_t seed = trueRandomGenerator();
    randomEngine.seed(seed);
}

void RandomStrategy::setRandomSeed(uint8_t randomSeed)
{
    randomEngine.seed(randomSeed);
}

size_t RandomStrategy::selectNextArm()
{
    return std::uniform_int_distribution<size_t>(0, armsCount_ - 1)(randomEngine);
}

void RandomStrategy::updateState(size_t, double)
{
    // do nothing
}

std::string RandomStrategy::getName()
{
    std::string str = "Random";
    return str;
}

std::vector<double> RandomStrategy::getArmsExpectation()
{
    // all arms have equal chances
    return std::vector<double>(armsCount_, 1.0);
}

DMED_BinaryStrategy::DMED_BinaryStrategy(size_t armsCount)
    : armsCount_(armsCount)
    , epsilonValue_(0.0001)
    , iterationNumber_(0)
{
    armPullsCount.reserve(armsCount);
    armTotalReward.reserve(armsCount);
    for(size_t i = 0; i < armsCount; ++i)
    {
        armPullsCount.push_back(0);
        armTotalReward.push_back(0);
        armsToPull_.insert(i);
    }
    LRc.clear();
    LN.clear();
}

size_t DMED_BinaryStrategy::selectNextArm()
{
    for(size_t i = 0; i < armsCount_; ++i)
    {
        // Firstly pull all arms at least once
        if(armPullsCount[i] == 0)
        {
            return i;
        }
    }

    if(armsToPull_.empty())
    {
        return 0;
    }
    return (*armsToPull_.begin());
}

void DMED_BinaryStrategy::updateState(size_t selectedArmIndex, double reward)
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

std::string DMED_BinaryStrategy::getName()
{
    std::string str = "DMED (binary)";
    return str;
}

std::vector<double> DMED_BinaryStrategy::getArmsExpectation()
{
    std::vector<double> ans;
    ans.reserve(armsCount_);
    for(size_t i = 0; i < armsCount_; ++i)
    {
        ans.push_back(std::max(std::min(armTotalReward[i] / armPullsCount[i], 1.0 - epsilonValue_), epsilonValue_));
    }
    return ans;
}

double DMED_BinaryStrategy::dualDivergence(double mu, double mu_opt)
{
    double nu = (mu_opt - mu) / ((1 - mu_opt) * mu_opt);
    return std::log(1 + mu_opt * nu) * (1 - mu) + std::log(1 - (1 - mu_opt) * nu) * mu;
}

KL_UCBStrategy::KL_UCBStrategy(size_t armsCount)
    : armsCount_(armsCount)
{
    Ni_ = std::vector<int>(armsCount_, 0);
    Gi_ = std::vector<double>(armsCount_, 0.0);
}

size_t KL_UCBStrategy::selectNextArm()
{
    /// KL-UCB indeces
    std::vector<double> indices = std::vector<double>(armsCount_, 0.0);
    const int n = std::accumulate(Ni_.begin(), Ni_.end(), 0);

    for(size_t currentArmIndex = 0; currentArmIndex < armsCount_; ++currentArmIndex)
    {
        if(Ni_[currentArmIndex] == 0)
        {
            return currentArmIndex;
        }
        indices[currentArmIndex] = getKLUCBUpper(currentArmIndex, n);
    }
    size_t maxElemtIndex = std::distance(indices.begin(), std::max_element(indices.begin(), indices.end()));
    return maxElemtIndex;
}

void KL_UCBStrategy::updateState(size_t selectedArmIndex, double reward)
{
    Ni_[selectedArmIndex] += 1;
    Gi_[selectedArmIndex] += reward;
}

std::string KL_UCBStrategy::getName()
{
    std::string str = "KL-UCB with c=0";
    return str;
}

std::vector<double> KL_UCBStrategy::getArmsExpectation()
{
    std::vector<double> ans;
    const int n = std::accumulate(Ni_.begin(), Ni_.end(), 0);

    ans.reserve(armsCount_);
    for(size_t currentArmIndex = 0; currentArmIndex < armsCount_; ++currentArmIndex)
    {
        if(Ni_[currentArmIndex] == 0)
        {
            ans.push_back(1);
        }
        else
        {
            ans.push_back(getKLUCBUpper(currentArmIndex, n));
        }
    }
    return ans;
}

double KL_UCBStrategy::getKLUCBUpper(size_t armIndex, int n)
{
    const double logNdn = log(n) / static_cast<double>(Ni_[armIndex]);
    const double p = std::max(Gi_[armIndex] / static_cast<double>(Ni_[armIndex]), delta_);
    if(p >= 1)
    {
        return 1;
    }
    bool converged = false;
    double q = p + delta_;
    for(int t = 0; (t < 20 && !converged); ++t)
    {
        const double f = logNdn - calculate_kl_divergence(p, q);
        const double df = -dkl(p, q);
        if(f * f < epsilon_)
        {
            converged = true;
            break;
        }
        q = std::min(1 - delta_, std::max(q - f / df, p + delta_));
    }
    if(!converged)
    {
        // TODO: log in trace/debug
        // std::cerr << "Newton iteration in KL-UCB algorithm did not converge";
    }
    return q;
}

double KL_UCBStrategy::calculate_kl_divergence(double p, double q)
{
    const double v = p * log(p / q) + (1 - p) * log((1 - p) / (1 - q));
    return v;
}

double KL_UCBStrategy::dkl(double p, double q)
{
    return (q - p) / (q * (1.0 - q));
}

} // namespace multiArmedBandit
