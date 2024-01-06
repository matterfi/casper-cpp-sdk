#include "Casper/Utils/CryptoUtil.h" // IWYU pragma: associated

#include <cryptopp/filters.h>
#include <ctime>

namespace Casper
{
CBytes CryptoUtil::hexDecode(std::string encoded)
{
    CStringSource ss(encoded, true, new CHexDecoder);
    CBytes decoded((size_t)ss.MaxRetrievable());
    ss.Get(decoded, decoded.size());
    return decoded;
}

std::string CryptoUtil::hexEncode(CBytes decoded)
{
    std::string encoded;

    CStringSource ss(decoded, decoded.size(), true,
                     new CHexEncoder(new CStringSink(encoded)) // HexEncoder
    );
    return encoded;
}

std::string CryptoUtil::timeToRFC3339(time_t time)
{
    std::tm tm;

#ifdef _WIN32
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif

    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S%z", &tm);
    return std::string(buffer);
}
} // namespace Casper
