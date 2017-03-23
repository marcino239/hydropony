#!/bin/bash

cd /tmp
curl -sL1 https://deb.nodesource.com/setup_7.x | sudo -E bash -
sudo apt install -y --no-install-recommends \
    nodejs \
    npm \
    sqlite3
sudo ln -s /usr/bin/nodejs /usr/bin/node
sudo mkdir -p /node/www
sudo chown node:node /node/www

