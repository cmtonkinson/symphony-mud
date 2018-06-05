#!/usr/bin/env ruby

require 'pathname'
BINARY_NAME = Pathname.new(__FILE__).basename

require_relative 'config/bootstrap'
require_relative 'config/dispatch'

