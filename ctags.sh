#!/bin/bash

ctags -e \
  --recurse \
  --exclude=test/state \
  --exclude=tmp \
  --exclude=coverage \
  --exclude=vendor

