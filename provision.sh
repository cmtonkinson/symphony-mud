#!/bin/bash

###############################################################################
# Functions
###############################################################################
# Most of the time we can get by with this DRY wrapper for sudo commands.
as_user() {
  echo "$USER_NAME:~$ > ${*}"
  su -l $USER_NAME -c "$*"
}

###############################################################################
# Base System
###############################################################################
apt-get -y update
apt-get -yfV dist-upgrade

# Install dependencies.
DEBIAN_FRONTEND=noninteractive apt-get install -yfV \
  g++ ccache                                        \
  libpcre3 libpcre3-dbg libpcre3-dev                \
  mysql-server mysql-client libmysqlclient-dev      \

###############################################################################
# Development Environment
###############################################################################
echo 'alias list="ls -alsvh"'     >> /home/vagrant/.bashrc
echo 'export MAKEFLAGS="-j4"'     >> /home/vagrant/.bashrc
echo 'export CCACHE_COMPRESS="1"' >> /home/vagrant/.bashrc
echo 'cd /vagrant'                >> /home/vagrant/.bashrc
