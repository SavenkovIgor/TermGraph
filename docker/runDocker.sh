#!/bin/bash
docker build -t term_graph_build_image .
docker run -it term_graph_build_image
