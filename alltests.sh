#!/bin/bash

time sh -c 'bundle exec rubocop --parallel \
  && bundle exec rspec --force-color \
  && bundle exec test/test.rb'

