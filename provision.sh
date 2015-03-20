#!/bin/bash

###############################################################################
# Base System
###############################################################################
apt-get -y update
apt-get -yfV dist-upgrade

DEBIAN_FRONTEND=noninteractive apt-get install -yfV \
  git clang ccache gdb                              \
  libpcre3 libpcre3-dbg libpcre3-dev                \
  libtool automake                                  \

###############################################################################
# Dependency: libsodium
# Thanks to http://askubuntu.com/a/215378
###############################################################################
cd /vagrant/vendor/libsodium
autoreconf --verbose --install --force
./configure
make
sudo make install
sudo ldconfig

###############################################################################
# Development Environment
###############################################################################
BASHRC=/home/vagrant/.bashrc
echo 'alias list="ls -alsvh"'           >> $BASHRC
echo 'alias c="telnet localhost 6501"'  >> $BASHRC
echo 'export MAKEFLAGS="-j4"'           >> $BASHRC
echo 'export CCACHE_COMPRESS=1'         >> $BASHRC
echo 'export CCACHE_CPP2=1'             >> $BASHRC
echo 'cd /vagrant'                      >> $BASHRC
