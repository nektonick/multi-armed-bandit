#pragma once

#include <set>
#include <string>
#include <vector>
#include "common_utils.h"

namespace multiArmedBandit {

class IStrategy
{
public:
    IStrategy() = default;
    virtual ~IStrategy() = default;
    virtual size_t selectNextArm() = 0;
    virtual void updateState(size_t selectedArmIndex, double reward) = 0;
    virtual std::string getName() = 0;
    virtual std::vector<double> getArmsExpectation() = 0;
};

class RandomStrategy : public IStrategy
{
public:
    RandomStrategy(size_t armsCount);
    size_t selectNextArm() override;
    void updateState(size_t selectedArmIndex, double reward) override;
    std::string getName() override;
    std::vector<double> getArmsExpectation() override;
    void setRandomSeed(uint8_t randomSeed);

protected:
    std::mt19937 randomEngine;
    const size_t armsCount_;
};

// Deterministic Minimum Empirical Divergence policy for binary rewards
class DMED_BinaryStrategy : public IStrategy
{
public:
    DMED_BinaryStrategy(size_t armsCount);

    size_t selectNextArm() override;
    void updateState(size_t selectedArmIndex, double reward) override;
    std::string getName() override;
    std::vector<double> getArmsExpectation() override;

protected: // internal math stuff
    double dualDivergence(double mu, double mu_opt);

protected:
    const size_t armsCount_;
    const double epsilonValue_;
    size_t iterationNumber_;

    std::vector<size_t> armPullsCount;
    std::vector<double> armTotalReward;

    std::set<size_t> armsToPull_;

    // some math naming
    std::set<size_t> LRc;
    std::set<size_t> LN;
};

/// KULLBACK–LEIBLER UPPER CONFIDENCE BOUNDS FOR OPTIMAL SEQUENTIAL ALLOCATION
class KL_UCBStrategy : public IStrategy
{
public:
    KL_UCBStrategy(size_t armsCount);

    size_t selectNextArm() override;
    void updateState(size_t selectedArmIndex, double reward) override;
    std::string getName() override;
    std::vector<double> getArmsExpectation() override;

protected: // internal math stuff
    double getKLUCBUpper(size_t armIndex, int n);
    double calculate_kl_divergence(double p, double q);
    double dkl(double p, double q);

protected:
    const size_t armsCount_;
    const double delta_ = 1e-8;
    const double epsilon_ = 1e-10;

    // some math naming
    std::vector<int> Ni_;
    std::vector<double> Gi_;
};

} // namespace multiArmedBandit
