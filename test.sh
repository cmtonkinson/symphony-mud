#!/bin/bash

bundle exec rubocop -P \
  && bundle exec rspec --force-color \
  && system-tests/test.rb

