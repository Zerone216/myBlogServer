#!/bin/bash

docker run -v$(pwd):/opt/my_blog --name my_blog -p 8080:8080 blog_build_docker
