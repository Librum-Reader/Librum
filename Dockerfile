FROM ubuntu:latest

RUN apt update -y && \
    apt install -y \
    build-essential \
    cmake \
    make \
    python3-venv \
    python3-pip \
    g++ \
    wget \
    libgl1-mesa-dev \
    libxcb-xinerama0 \
    libxkbcommon-dev \
    libglib2.0-0 \
    libdbus-1-3

RUN pip install aqtinstall && \
    aqt install-qt linux desktop 6.5


COPY . /app/

WORKDIR /app/build-Release

RUN cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=Off -DCMAKE_PREFIX_PATH=/6.5.2/gcc_64 .. && \
    cmake --build . -j 1 && \
    cmake --install .

CMD ["./librum"]