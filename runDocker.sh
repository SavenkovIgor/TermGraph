#!/bin/bash
docker run --rm -it --mount src=$PWD,target=/root/TermGraph,type=bind tg_build
