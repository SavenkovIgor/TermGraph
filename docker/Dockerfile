FROM ubuntu:20.10
LABEL description="Docker for standalone build of TermGraph"
RUN apt update && \
    apt install -y pip git vim gcc-10 g++-10 && \
    pip install conan && \
    cd /root && \
    git clone https://gitlab.com/SavenkovIgor/TermGraph.git && \
    cd TermGraph && \
    conan install -if ./conan -pr ./conan/profiles/linux ./
