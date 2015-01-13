#!/bin/bash

# $Id: keep_alive.sh 433 2010-07-24 11:32:32Z cmtonkinson@gmail.com $
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

ROOT="."

while [ 1 ]; do
  # Run Symphony and log all output...
    $ROOT/symphony >>$ROOT/log/out.log 2>&1
  # Get a timestamp...
    DATE=`date +%Y-%m-%d-%H-%M-%S`
    CORE_DIR="$ROOT/cores/$DATE"
    PROF_DIR="$ROOT/profiling"
  # Save the core file & executable...
    if [ -f core.* ]; then
      mkdir -p $CORE_DIR
      mv core.* "$CORE_DIR/core"
      cp ./symphony "$CORE_DIR/symphony"
    fi
  # If there's a gmon.out, stare the profiler data...
    if [ -f gmon.out ]; then
      mkdir -p $PROF_DIR
      gprof ./symphony > "$PROF_DIR/profile-$DATE.txt"
    fi
  # Take a breather...
    sleep 1s
done
