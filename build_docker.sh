#!/bin/bash

version=$(cat VERSION)
docker build -t hydropony:$version -f Dockerfile .

