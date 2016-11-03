#! /bin/sh

rm *.png

DB=pg_morton_pc
dropdb $DB
createdb $DB

psql -d $DB -f init.sql
pdal pipeline -i pipe.json
psql -d $DB -f indexing.sql

python3 graph.py
