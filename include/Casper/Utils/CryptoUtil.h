#pragma once

#include <ctime>
#include <string>

#include "Casper/Base.h"

namespace Casper
{
struct CryptoUtil
{
    static CBytes hexDecode(std::string hex);

    static std::string hexEncode(CBytes bytes);

    static std::string timeToRFC3339(time_t time);
};

} // namespace Casper
