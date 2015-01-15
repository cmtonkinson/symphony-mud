#!/bin/bash

source bin/database-creds.sh

mysqldump --no-data        $MYSQL_CREDS > db/schema.sql
mysqldump --no-create-info $MYSQL_CREDS > db/seed.sql
