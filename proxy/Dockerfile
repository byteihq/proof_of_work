FROM ubuntu:22.04 as builder

RUN apt-get update && apt-get install -y cmake g++ git \
libjsoncpp-dev uuid-dev zlib1g-dev libhiredis-dev

COPY . /build
WORKDIR /build

RUN cmake -S . -B . -DCMAKE_BUILD_TYPE=Test -DBUILD_POSTGRESQL=0 -DBUILD_MYSQL=0 -DBUILD_SQLITE=0 -DBUILD_REDIS=0 -DBUILD_TESTING=0 -DBUILD_EXAMPLES=0 -DBUILD_CTL=0
RUN cmake --build .


FROM ubuntu:22.04 as runner

RUN apt-get update && apt-get install -y \
libjsoncpp-dev uuid-dev zlib1g-dev libhiredis-dev

COPY --from=builder /build/proxy_app /app/run/
COPY --from=builder /build/config.json /app/run/
COPY --from=builder /build/_deps/redis++-build/libredis++.so.1 /usr/lib/


WORKDIR /app/run
