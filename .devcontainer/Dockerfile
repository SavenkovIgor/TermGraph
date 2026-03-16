FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-24.04

# Path env variables
ENV WORKSPACE_ROOT=/workspaces
ENV WORKSPACE=${WORKSPACE_ROOT}/TermGraph
ENV QT_ROOT=${WORKSPACE_ROOT}/Qt
ENV EMSDK_ROOT=${WORKSPACE_ROOT}/emsdk

RUN apt update                                && \
    # Qt dependencies
    apt install -y software-properties-common && \
    apt install -y libgl1-mesa-dev            && \
    apt install -y libxkbcommon-dev           && \
    apt install -y libfontconfig1             && \
    apt install -y libfreetype6               && \
    # python dependencies
    apt install -y python3-pip python3-venv   && \
    # uv dependencies
    apt install -y ca-certificates gpg wget

# uv installation
ARG UV_INSTALL_DIR=/usr/local/bin
ADD https://astral.sh/uv/install.sh /uv-installer.sh
RUN sh /uv-installer.sh && rm /uv-installer.sh

# Configure qt and aqt installations
ARG QT_VERSION=6.8.3
ARG AQT_VERSION=3.3.0
ENV QT_VERSION=${QT_VERSION}
COPY aqt.ini ${WORKSPACE_ROOT}/aqt.ini
ENV AQT_CONFIG=${WORKSPACE_ROOT}/aqt.ini

# Install Qt gcc_64
RUN uvx --from aqtinstall@$AQT_VERSION aqt install-qt -O ${QT_ROOT} linux desktop ${QT_VERSION} linux_gcc_64 \
    --modules qtimageformats qtshadertools \
    --archives qttranslations qttools qtsvg qtdeclarative qtbase icu

# Install Qt wasm multithread
RUN uvx --from aqtinstall@$AQT_VERSION aqt install-qt -O ${QT_ROOT} all_os wasm ${QT_VERSION} wasm_singlethread \
    --modules qtimageformats qtshadertools \
    --archives qttranslations qttools qtsvg qtdeclarative qtbase icu

# Install emscripten
ARG EMSDK_VERSION=3.1.56

RUN git clone https://github.com/emscripten-core/emsdk.git ${EMSDK_ROOT} && \
    cd ${EMSDK_ROOT}                                                     && \
    ./emsdk install ${EMSDK_VERSION}                                     && \
    ./emsdk activate ${EMSDK_VERSION}                                    && \
    echo "source ${EMSDK_ROOT}/emsdk_env.sh" >> /home/vscode/.bashrc

ARG CLANG_VERSION=21

RUN cd /home/vscode                   && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh                  && \
    ./llvm.sh ${CLANG_VERSION} all    && \
    rm llvm.sh

# Make this clang version the default one
ENV PATH="/usr/lib/llvm-${CLANG_VERSION}/bin:${PATH}"

# Provide cc/c++ system links (not included in LLVM bin dir)
RUN update-alternatives --install /usr/bin/cc  cc  /usr/bin/clang-${CLANG_VERSION}   100 && \
    update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++-${CLANG_VERSION} 100

# Installing conan with uv tool
# We are switching user because we want the conan installation
# to be available for the vscode user
ARG CONAN_VERSION=2.25.2
USER vscode
RUN uv tool install conan@${CONAN_VERSION}
USER root

# Install GDB for debugging support
RUN apt update && apt install -y gdb
