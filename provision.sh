#!/bin/bash

###############################################################################
# Functions
###############################################################################
# Most of the time we can get by with this DRY wrapper for sudo commands.
as_user() {
  echo "$USER_NAME:~$ > ${*}"
  su -l $USER_NAME -c "$*"
}

mysql_exec() {
  echo "mysql: ${*}"
  mysql -u root -e "$*"
}

###############################################################################
# Base System
###############################################################################
apt-get -y update
apt-get -yfV dist-upgrade

# Install dependencies.
DEBIAN_FRONTEND=noninteractive apt-get install -yfV \
  g++ ccache gdb                                    \
  libpcre3 libpcre3-dbg libpcre3-dev                \
  mysql-server mysql-client libmysqlclient-dev      \

###############################################################################
# MySQL Server
###############################################################################
mysql_exec "CREATE USER 'symphony'@'localhost' IDENTIFIED BY 'secure';"
mysql_exec "CREATE DATABASE symphony;"
mysql_exec "GRANT ALL PRIVILEGES ON symphony.* TO 'symphony'@'localhost';"

###############################################################################
# Development Environment
###############################################################################
echo 'alias list="ls -alsvh"'     >> /home/vagrant/.bashrc
echo 'export MAKEFLAGS="-j4"'     >> /home/vagrant/.bashrc
echo 'export CCACHE_COMPRESS=1'   >> /home/vagrant/.bashrc
echo 'cd /vagrant'                >> /home/vagrant/.bashrc

###############################################################################
# Symphony Setup
###############################################################################
as_user "cd /vagrant && bash bin/migrate.sh"
