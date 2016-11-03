#! /bin/sh

rm *.png

DB=pg_morton_graph
dropdb $DB
createdb $DB

psql -d $DB -f init.sql
python3 graph.py
