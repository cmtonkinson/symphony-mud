#!/bin/bash

source bin/database-creds.sh

mysql $MYSQL_CREDS < db/schema.sql
mysql $MYSQL_CREDS < db/seed.sql
