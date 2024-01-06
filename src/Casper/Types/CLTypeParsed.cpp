#include "Casper/Types/CLTypeParsed.h" // IWYU pragma: associated

namespace Casper
{
bool CLTypeParsed::operator<(const CLTypeParsed& b) const
{
    return true;
}
bool CLTypeParsed::operator<(CLTypeParsed&& b) const
{
    return true;
}

} // namespace Casper
