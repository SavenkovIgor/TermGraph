#!/bin/bash
docker rm $(docker ps -a -q -f ancestor=term_graph_build_image)
docker image rm term_graph_build_image
