#!/bin/bash

bundle exec rubocop -P \
  && bundle exec rspec --force-color \
  && test/test.rb

