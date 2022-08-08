#!/bin/bash
docker rm $(docker ps -a -q -f ancestor=tg_build)
docker image rm tg_build
