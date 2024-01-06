#pragma once

#include "Casper/Base.h"
#include "Casper/ByteSerializers/BaseByteSerializer.h"
#include "Casper/Types/DeployApproval.h"
#include "Casper/Types/PublicKey.h"
#include "Casper/Types/Signature.h"

namespace Casper
{
struct DeployApprovalByteSerializer : public BaseByteSerializer
{
    CBytes ToBytes(DeployApproval& source)
    {
        CBytes bytes;
        WriteBytes(bytes, source.signer.GetBytes());
        WriteBytes(bytes, source.signature.GetBytes());
        return bytes;
    }
};

} // namespace Casper