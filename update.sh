#!/bin/bash
git pull
git submodule foreach git pull
./make_docker.sh
docker-compose down
docker-compose up -d
