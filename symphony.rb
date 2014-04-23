#!/usr/bin/env ruby
BINARY_NAME = __FILE__
require File.expand_path 'engine/bootstrap'
$universe = Base::Universe.new
$universe.live
