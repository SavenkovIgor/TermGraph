FROM ubuntu:20.10
LABEL description="Docker for build of TermGraph"
RUN mkdir /root/TermGraph
WORKDIR /root/TermGraph
RUN apt update && \
    apt install -y vim gcc-10 g++-10 cmake pip && \
    apt install -y libgl1-mesa-dev pkg-config xorg-dev libx11-xcb-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev libxcb-util-dev && \
    pip3 install conan

#CMD ["bash", "buildConan.sh"]
