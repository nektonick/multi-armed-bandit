#pragma once

#include <string>

namespace multiArmedBandit {

/// Arm interface with virtual functions
class IArm
{
public:
    virtual double pull() = 0;
    virtual double getExpectedReward() = 0;
    virtual std::string toString() = 0;
};

} // namespace multiArmedBandit
