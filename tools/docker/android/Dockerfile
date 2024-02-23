# syntax=docker/dockerfile:1

ARG NDK_VERSION
ARG UPSTREAM_ANDROID_VERSION

FROM opentransactions/android:${UPSTREAM_ANDROID_VERSION} AS final

COPY --from=files casper_sdk_build /usr/bin
ENTRYPOINT [ "/usr/bin/casper_sdk_build" ]
CMD []
