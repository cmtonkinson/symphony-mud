#!/bin/bash

# $Id: run_tests.sh 435 2010-07-24 11:37:54Z cmtonkinson@gmail.com $
#
# This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
# Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEST_DIR="test/bin"

for utest in `ls $TEST_DIR/*_utest`; do
  $utest
  if [ $? -ne 0 ]; then
    exit
  fi
done
