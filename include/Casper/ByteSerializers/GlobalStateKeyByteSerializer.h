#pragma once

#include <cryptopp/config_int.h>
#include <string>

#include "Casper/Base.h"
#include "Casper/ByteSerializers/BaseByteSerializer.h"
#include "Casper/Types/GlobalStateKey.h"
#include "Casper/Types/URef.h"

namespace Casper
{
struct GlobalStateKeyByteSerializer : public BaseByteSerializer
{
    CBytes ToBytes(GlobalStateKey& source)
    {
        CBytes bytes;

        WriteByte(bytes, (CryptoPP::byte)source.key_identifier);

        WriteBytes(bytes, source.raw_bytes);

        if (source.ToString().find("uref-") != std::string::npos)
            {
                URef uref(source.ToString());
                WriteByte(bytes, (CryptoPP::byte)uref.access_rights);
            }

        return bytes;
    }
};

} // namespace Casper