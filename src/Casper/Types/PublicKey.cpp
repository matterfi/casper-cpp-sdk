#include "Casper/Types/PublicKey.h" // IWYU pragma: associated

namespace Casper
{
Casper::PublicKey PublicKey::FromPemFile(const std::string& pemFilePath)
{
    throw std::runtime_error("Not implemented: static Casper::PublicKey FromPemFile(const "
                             "std::string& pemFilePath)");
    // Optional TODO: Implement this

    /*
    try {
        CryptoPP::FileSource fs1(pemFilePath.c_str(), true);
        if (CryptoPP::PEM_GetType(fs1) == CryptoPP::PEM_EC_PUBLIC_KEY) {
        CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;

        publicKey.Load(fs1);

        // TODOMS3: Check the false below if any error occurs
        CBytes rawBytes(
            publicKey.GetGroupParameters().GetEncodedElementSize(false));

        // TODOMS3: Check the false below if any error occurs
        publicKey.GetGroupParameters().EncodeElement(
            false, publicKey.GetPublicElement(), rawBytes);

        return PublicKey(rawBytes, KeyAlgo::SECP256K1);

        } else {
        CryptoPP::ed25519PublicKey k1;
        k1.Load(fs1);
        CBytes rawBytes(k1.GetPublicElement().MinEncodedSize());
        k1.GetPublicElement().Encode(rawBytes, rawBytes.size());
        return PublicKey(rawBytes, KeyAlgo::ED25519);
        }
    } catch (std::exception& e) {
        SPDLOG_ERROR("Unsupported key format or it's not a public key PEM object.");
    }
    return {};
    */
}

void PublicKey::WriteToPem(const std::string& filePath)
{
    throw std::runtime_error("Not implemented: void WriteToPem(const std::string& filePath)");
    // Optional TODO: Implement this

    /*
    if (File::Exists(filePath))
        throw std::runtime_error(
            "Target file already exists. Will not overwrite.\nFile: " + filePath);

        CryptoPP::ECDSA<ECP, SHA256>::PublicKey publicKey;
        publicKey.Initialize(CryptoPP::ASN1::secp256k1(), raw_bytes.begin(),
                                raw_bytes.size());

        using(var textWriter = File.CreateText(filePath)) {
            var writer = new PemWriter(textWriter);

            if (key_algorithm == KeyAlgo.ED25519) {
            Ed25519PublicKeyParameters pk =
                new Ed25519PublicKeyParameters(RawBytes, 0);
            writer.WriteObject(pk);
            return;
            } else if (key_algorithm == KeyAlgo.SECP256K1) {
            var curve = ECNamedCurveTable.GetByName("secp256k1");
            var domainParams = new ECDomainParameters(curve.Curve, curve.G,
        curve.N, curve.H, curve.GetSeed());

            ECPoint q = curve.Curve.DecodePoint(RawBytes);
            ECPublicKeyParameters pk = new ECPublicKeyParameters(q,
        domainParams); writer.WriteObject(pk); return;
            }

            throw std::runtime_error("Unsupported key type.");
        }
    */
}

} // namespace Casper
