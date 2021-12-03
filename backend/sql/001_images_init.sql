CREATE TABLE images (
  id                  INTEGER PRIMARY KEY,
  algo                TEXT NOT NULL CHECK(algo IN ("CGNR", "CGNE")),
  start_date          TEXT,
  end_date            TEXT,
  data                TEXT,
  height              INTEGER,
  width               INTEGER,
  iterations          INTEGER,
  quality             INTEGER,
  time                REAL,
  user                INTEGER NOT NULL,
  FOREIGN KEY(user)   REFERENCES users(id)
);