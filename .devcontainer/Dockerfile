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
    # Cmake, Ninja and uv dependencies
    apt install -y ca-certificates gpg wget unzip curl

# uv installation
ENV UV_INSTALL_DIR=/usr/local/bin
ADD https://astral.sh/uv/install.sh /uv-installer.sh
RUN sh /uv-installer.sh && rm /uv-installer.sh

# Configure qt and aqt installations
ENV QT_VERSION=6.8.3
ENV AQT_VERSION=3.2.1
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
ENV EMSDK_VERSION=3.1.56

RUN git clone https://github.com/emscripten-core/emsdk.git ${EMSDK_ROOT} && \
    cd ${EMSDK_ROOT}                                                     && \
    ./emsdk install ${EMSDK_VERSION}                                     && \
    ./emsdk activate ${EMSDK_VERSION}                                    && \
    echo "source ${EMSDK_ROOT}/emsdk_env.sh" >> /home/vscode/.bashrc

ENV CLANG_VERSION=20

RUN cd /home/vscode                   && \
    wget https://apt.llvm.org/llvm.sh && \
    chmod +x llvm.sh                  && \
    ./llvm.sh ${CLANG_VERSION} all    && \
    rm llvm.sh

RUN update-alternatives \
    --verbose \
    --install /usr/bin/clang                    clang                    /usr/bin/clang-${CLANG_VERSION} 100                \
    --slave   /usr/bin/clang++                  clang++                  /usr/bin/clang++-${CLANG_VERSION}                  \
    --slave   /usr/bin/clang-apply-replacements clang-apply-replacements /usr/bin/clang-apply-replacements-${CLANG_VERSION} \
    --slave   /usr/bin/clang-change-namespace   clang-change-namespace   /usr/bin/clang-change-namespace-${CLANG_VERSION}   \
    --slave   /usr/bin/clang-check              clang-check              /usr/bin/clang-check-${CLANG_VERSION}              \
    --slave   /usr/bin/clang-cl                 clang-cl                 /usr/bin/clang-cl-${CLANG_VERSION}                 \
    --slave   /usr/bin/clang-cpp                clang-cpp                /usr/bin/clang-cpp-${CLANG_VERSION}                \
    --slave   /usr/bin/clangd                   clangd                   /usr/bin/clangd-${CLANG_VERSION}                   \
    --slave   /usr/bin/clang-doc                clang-doc                /usr/bin/clang-doc-${CLANG_VERSION}                \
    --slave   /usr/bin/clang-extdef-mapping     clang-extdef-mapping     /usr/bin/clang-extdef-mapping-${CLANG_VERSION}     \
    --slave   /usr/bin/clang-format             clang-format             /usr/bin/clang-format-${CLANG_VERSION}             \
    --slave   /usr/bin/clang-include-cleaner    clang-include-cleaner    /usr/bin/clang-include-cleaner-${CLANG_VERSION}    \
    --slave   /usr/bin/clang-include-fixer      clang-include-fixer      /usr/bin/clang-include-fixer-${CLANG_VERSION}      \
    --slave   /usr/bin/clang-installapi         clang-installapi         /usr/bin/clang-installapi-${CLANG_VERSION}         \
    --slave   /usr/bin/clang-linker-wrapper     clang-linker-wrapper     /usr/bin/clang-linker-wrapper-${CLANG_VERSION}     \
    --slave   /usr/bin/clang-move               clang-move               /usr/bin/clang-move-${CLANG_VERSION}               \
    --slave   /usr/bin/clang-nvlink-wrapper     clang-nvlink-wrapper     /usr/bin/clang-nvlink-wrapper-${CLANG_VERSION}     \
    --slave   /usr/bin/clang-offload-bundler    clang-offload-bundler    /usr/bin/clang-offload-bundler-${CLANG_VERSION}    \
    --slave   /usr/bin/clang-offload-packager   clang-offload-packager   /usr/bin/clang-offload-packager-${CLANG_VERSION}   \
    --slave   /usr/bin/clang-query              clang-query              /usr/bin/clang-query-${CLANG_VERSION}              \
    --slave   /usr/bin/clang-refactor           clang-refactor           /usr/bin/clang-refactor-${CLANG_VERSION}           \
    --slave   /usr/bin/clang-reorder-fields     clang-reorder-fields     /usr/bin/clang-reorder-fields-${CLANG_VERSION}     \
    --slave   /usr/bin/clang-repl               clang-repl               /usr/bin/clang-repl-${CLANG_VERSION}               \
    --slave   /usr/bin/clang-scan-deps          clang-scan-deps          /usr/bin/clang-scan-deps-${CLANG_VERSION}          \
    --slave   /usr/bin/clang-sycl-linker        clang-sycl-linker        /usr/bin/clang-sycl-linker-${CLANG_VERSION}        \
    --slave   /usr/bin/clang-tblgen             clang-tblgen             /usr/bin/clang-tblgen-${CLANG_VERSION}             \
    --slave   /usr/bin/clang-tidy               clang-tidy               /usr/bin/clang-tidy-${CLANG_VERSION}

# Installing conan with uv tool
# We are switching user because we want the conan installation
# to be available for the vscode user
ENV CONAN_VERSION=2.16.1
USER vscode
RUN uv tool install conan@${CONAN_VERSION}
USER root

# Install the latest CMake using Kitware's helper script
RUN wget -O - https://apt.kitware.com/kitware-archive.sh | bash && \
    apt update && \
    apt install -y cmake && \
    rm -rf /var/lib/apt/lists/*

# Install the latest Ninja manually
RUN wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip && \
    unzip ninja-linux.zip -d /usr/bin/                                                  && \
    rm ninja-linux.zip                                                                  && \
    chmod +x /usr/bin/ninja

# Install GDB for debugging support
RUN apt update && apt install -y gdb
