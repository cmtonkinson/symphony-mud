#!/usr/bin/env ruby

require 'pathname'
BINARY_NAME = Pathname.new(__FILE__).basename

require_relative 'lib/app'
require_relative 'boot/bootstrap'
require_relative 'boot/dispatch'
