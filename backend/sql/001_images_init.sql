CREATE TABLE images (
  id                  INTEGER PRIMARY KEY,
  algo                TEXT NOT NULL CHECK(algo IN ("CGNR", "CGNE")),
  start_date          TEXT,
  end_date            TEXT,
  data                TEXT,  -- Trocar isso para blob talvez
  height              INTEGER,
  width               INTEGER,
  iterations          INTEGER,
  time                REAL,
  user                INTEGER,
  FOREIGN KEY(user) REFERENCES users(id)
);