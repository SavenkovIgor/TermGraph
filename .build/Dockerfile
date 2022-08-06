FROM ubuntu:20.10
LABEL description="Docker for build of TermGraph"
RUN mkdir /root/TermGraph
WORKDIR /root/TermGraph
RUN apt update && \
    apt install -y vim gcc-10 g++-10 cmake make python pip && \
    apt install -y libgl1-mesa-dev pkg-config xorg-dev libx11-xcb-dev libxcb*-dev xkb-data libfontconfig1-dev libxkbcommon-x11-dev && \
    pip3 install conan

#CMD ["bash", "buildConan.sh"]
