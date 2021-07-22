#!/bin/bash
docker build -t term_graph_build_image -f Dockerfile.linux .
docker run -it term_graph_build_image
