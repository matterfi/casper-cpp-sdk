#pragma once

#include <string>
#include <vector>

#include "Casper/Base.h"

namespace Casper
{
class CEP57Checksum
{
private:
    static CBytes _bytes_to_nibbles(CBytes bytes);

    static std::vector<bool> _bytes_to_bits_cycle(CBytes bytes);

public:
    static bool HasChecksum(std::string hex);

    static std::string Encode(CBytes decoded);

    static CBytes Decode(std::string encoded);
};

} // namespace Casper
