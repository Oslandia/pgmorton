from itertools import chain
from psycopg2 import connect
from psycopg2.extras import NamedTupleCursor

class Session(object):

    def __init__(self):
        self.PG_USER = "blottiere"
        self.PG_PASSWORD = ""
        self.PG_HOST = "localhost"
        self.PG_PORT = 5432
        self.PG_NAME = "pg_morton_graph"
        self.PG_TABLE = "morton"
        self.PG_COLUMN = "myindex"

        self.db = connect(
                "postgresql://{0}:{1}@{2}:{3}/{4}"
                .format(self.PG_USER, self.PG_PASSWORD, self.PG_HOST,
                    self.PG_PORT, self.PG_NAME),
                cursor_factory=NamedTupleCursor,
                )

        self.db.autocommit = True

    def insert(self, x, y):
        sql = ("insert into {0} values(morton_encode({1}, {2}));"
               .format(self.PG_TABLE, x, y))
        self.db.cursor().execute(sql)

    def first_morton(self, n):
        sql = ("select * from {0} order by {1} ASC limit {2}"
               .format(self.PG_TABLE, self.PG_COLUMN, n))
        l = self._query_aslist(sql)
        return l

    def first_revert_morton(self, n):
        sql = ("select * from {1}"
               " order by morton_revert({0}) limit {2}"
               .format(self.PG_COLUMN, self.PG_TABLE, n))
        l = self._query_aslist(sql)
        return l

    def decode_morton(self, m):
        sql = ("select morton_decode({0});"
               .format(m))
        return self._query_aslist(sql)

    def _query(self, query, parameters=None):

        cur = self.db.cursor()
        cur.execute(query, parameters)
        if not cur.rowcount:
            return None
        for row in cur:
            yield row

    def _query_asdict(self, query, parameters=None):
        return [
            line._asdict()
            for line in self._query(query, parameters=parameters)
        ]

    def _query_aslist(self, query, parameters=None):
        return list(chain(*self._query(query, parameters=parameters)))
