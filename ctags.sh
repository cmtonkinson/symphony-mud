#!/bin/bash

ctags -e \
  --recurse \
  --exclude=tmp \
  --exclude=coverage

