#pragma once

#include <set>
#include <string>
#include <vector>

#include <algorithm>
#include <cmath>

namespace multiArmedBandit {

class IStrategy
{
public:
    virtual int selectNextArm() = 0;
    virtual void updateState(int selectedArmIndex, double reward) = 0;
    virtual std::string toString() = 0;
};

class RandomStrategy : public IStrategy
{
public:
    RandomStrategy(size_t armsCount);
    int selectNextArm() override;
    void updateState(int selectedArmIndex, double reward) override;
    std::string toString() override;

protected:
    const size_t armsCount_;
};

//DMED policy for binary rewards
class DMEDBinaryStrategy : public IStrategy
{
public:
    DMEDBinaryStrategy(size_t armsCount);

    int selectNextArm() override;
    void updateState(int selectedArmIndex, double reward) override;
    std::string toString() override;

protected:
    double dualDivergence(double mu, double mu_opt);

protected:
    const size_t armsCount_;
    const double epsilonValue_;
    size_t iterationNumber_;

    std::vector<size_t> armPullsCount;
    std::vector<double> armTotalReward;

    std::set<int> armsToPull_;
    // I have no idea, what LRc and LN mean
    std::set<int> LRc; //Note: LRc is the complementary set of L_R
    std::set<int> LN;
};

} // namespace multiArmedBandit
