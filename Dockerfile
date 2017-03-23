FROM debian:jessie
LABEL maintainer=marcino239@gmail.com

RUN groupadd --gid 1000 node \
    && useradd --uid 1000 --gid node --shell /bin/bash --create-home node

RUN apt-get update && apt-get install -y --no-install-recommends \
        curl \
        sudo

COPY install.sh /tmp/

RUN DEBIAN_FRONTEND=noninteractive /tmp/install.sh \
    && rm -rf /var/lib/apt/lists/*

COPY package.json /node/www/
WORKDIR /node/www

RUN npm install \
    && sqlite3 piTemps.db 'CREATE TABLE temperature_records(unix_time bigint primary key, celsius real);'

COPY server.js temperature_log.htm temperature_plot.htm /node/www/

RUN chown -R node:node /node

EXPOSE 8000
USER node
CMD [ "npm", "start" ]

