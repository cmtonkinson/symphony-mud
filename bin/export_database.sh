#!/bin/bash

mysqldump -u symphony --password=secure --no-data         symphony > db/schema.sql
mysqldump -u symphony --password=secure --no-create-info  symphony > db/seed.sql
