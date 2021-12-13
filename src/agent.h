#pragma once

#include "strategies.h"
#include <memory> // shared_ptr
#include <vector>

namespace multiArmedBandit {

using StrategyPtr = std::shared_ptr<IStrategy>;

class Agent
{
public:
protected:
    std::vector<StrategyPtr> strategies_;
};

} // namespace multiArmedBandit
