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
  clang ccache gdb                                  \
  libpcre3 libpcre3-dbg libpcre3-dev                \

###############################################################################
# libsodium
###############################################################################


###############################################################################
# Development Environment
###############################################################################
echo 'alias list="ls -alsvh"'           >> /home/vagrant/.bashrc
echo 'alias c="telnet localhost 6501"'  >> /home/vagrant/.bashrc
echo 'export MAKEFLAGS="-j4"'           >> /home/vagrant/.bashrc
echo 'export CCACHE_COMPRESS=1'         >> /home/vagrant/.bashrc
echo 'export CCACHE_CPP2=1'             >> /home/vagrant/.bashrc
echo 'cd /vagrant'                      >> /home/vagrant/.bashrc
