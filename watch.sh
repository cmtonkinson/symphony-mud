#!/bin/bash
watch -c -n 0 'ctags -Re && bundle exec rspec --force-color'

