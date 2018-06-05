#!/bin/bash
#
# This script is just a developer convenience. It keeps the TAGS file
# up-to-date and also continually runs project tests.

watch -c -n 0.1 'ctags -Re && bundle exec rspec --force-color'

