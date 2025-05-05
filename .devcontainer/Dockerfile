FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-24.04

# Path env variables
ENV WORKSPACE_ROOT=/workspaces
ENV WORKSPACE=${WORKSPACE_ROOT}/TermGraph
ENV VENV_BIN=${WORKSPACE_ROOT}/.venv/bin
ENV QT_ROOT=${WORKSPACE_ROOT}/Qt
ENV EMSDK_ROOT=${WORKSPACE_ROOT}/emsdk

RUN apt update && apt upgrade -y              && \
    apt install -y software-properties-common && \
    apt install -y libgl1-mesa-dev            && \
    apt install -y libxkbcommon-dev           && \
    apt install -y libfontconfig1             && \
    apt install -y libfreetype6

ENV CLANG_VERSION=20

RUN cd /home/vscode                   && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh                  && \
    ./llvm.sh ${CLANG_VERSION} all    && \
    rm llvm.sh

RUN apt install -y clang-format-${CLANG_VERSION} && \
    apt install -y clang-tools-${CLANG_VERSION}

# Install pip and venv
RUN apt install -y python3-pip python3-venv

# Create and activate virtual environment, install packages locally
ENV CONAN_VERSION=2.16.1
ENV AQT_VERSION==3.2.0

RUN python3 -m venv $WORKSPACE_ROOT/.venv                                    && \
    $VENV_BIN/python -m pip install --upgrade pip                            && \
    $VENV_BIN/pip install conan==${CONAN_VERSION} aqtinstall==${AQT_VERSION}

# Install Qt gcc_64
ENV QT_VERSION=6.6.3

RUN $VENV_BIN/aqt install-qt -O ${QT_ROOT} linux desktop ${QT_VERSION} gcc_64 \
    --modules qtimageformats qt5compat qtshadertools \
    --archives qttranslations qttools qtsvg qtdeclarative qtbase icu

# Install Qt wasm multithread
RUN $VENV_BIN/aqt install-qt -O ${QT_ROOT} linux desktop ${QT_VERSION} wasm_singlethread \
    --modules qtimageformats qt5compat qtshadertools \
    --archives qttranslations qttools qtsvg qtdeclarative qtbase icu

# Install emscripten
ENV EMSDK_VERSION=3.1.37

RUN git clone https://github.com/emscripten-core/emsdk.git ${EMSDK_ROOT} && \
    cd ${EMSDK_ROOT}                                                     && \
    ./emsdk install ${EMSDK_VERSION}                                     && \
    ./emsdk activate ${EMSDK_VERSION}                                    && \
    echo "source ${EMSDK_ROOT}/emsdk_env.sh" >> /home/vscode/.bashrc

# Cmake installation
# 1. Install prerequisites
RUN apt install -y --no-install-recommends ca-certificates gpg wget

# 2. Manually add the Kitware signing key if not already provided by the kitware-archive-keyring package.
#    (The official instructions suggest checking for a file in /usr/share/doc/kitware-archive-keyring/)
RUN test -f /usr/share/doc/kitware-archive-keyring/copyright || \
    wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | \
    gpg --dearmor -o /usr/share/keyrings/kitware-archive-keyring.gpg

# 3. Add the Kitware APT repository using the signed-by option.
RUN echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ jammy main' \
    > /etc/apt/sources.list.d/kitware.list

# 4. Update package lists and install the kitware-archive-keyring package.
#    (This step ensures that the keyring stays up to date; if the key wasn't installed via the package,
#     the instructions require removing the manually obtained key to avoid conflicts.)
RUN apt update && apt install -y kitware-archive-keyring && \
    test -f /usr/share/doc/kitware-archive-keyring/copyright || \
    rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
    rm -rf /var/lib/apt/lists/*

# 5. Finally, install the latest CMake and Ninja from the Kitware repository.
RUN apt update && apt install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Install the latest Ninja manually
RUN wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip && \
    unzip ninja-linux.zip -d /usr/bin/                                                  && \
    rm ninja-linux.zip                                                                  && \
    chmod +x /usr/bin/ninja

RUN echo 'if [ -f $VENV_BIN/activate ]; then \
            source $VENV_BIN/activate; \
          fi' >> /home/vscode/.bashrc
