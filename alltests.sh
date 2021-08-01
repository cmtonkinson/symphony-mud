#!/bin/bash

bundle exec rubocop -P \
  && bundle exec rspec --force-color \
  && bundle exec test/test.rb

