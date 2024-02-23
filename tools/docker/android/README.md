# CASPER SDK Android Build Image

This image creates an Android NDK development environment and compiles all opentxs dependencies. A build script is included which will compile opentxs for a specified architecture and bindings.

## Contents

* Android SDK
  * Platform (android-33)
  * Android NDK (26.0.10792818)
* Boost (1.84)
* Googletest (1.14.0)
* LMDB (8d0cbbc936091eb85972501a9b31a8f86d4c51a7)
* OpenSSL (3.1.4)
* otcommon (3.0.0)
* Libsodium (1.0.18)
* Protocol Buffers (23.4)
* Qt (6.6.1)
* secp256k1 (0.4.1)
* ZeroMQ (4.3.4)

## Usage

### Building the image

Run the following command in the parent directory to this README.md file:

```
docker buildx bake -f docker-bake.hcl android
```

### Compiling casper sdk

Valid architectures (first argument, mandatory): arm64 arm x64 x86

Valid bindings (second argument, optional): none qt all


#### Example

```
docker run \
  --tmpfs /tmp/cspr_sdk/build:rw,nosuid,size=2g \
  --mount type=bind,src=/home/bartek/repo/android_sdk,dst=/home/cspr_sdk/src \
  --mount type=bind,src=/home/bartek/repo/android_sdk/output,dst=/home/cspr_sdk/output \
  -it docker.matterfi.net/android-wallet:latest \
  arm64 \
  all
```
