CREATE TABLE users (
  id                  INTEGER PRIMARY KEY,
  username            TEXT UNIQUE NOT NULL 
);

CREATE TABLE images (
  id                  INTEGER PRIMARY KEY,
  algo                TEXT CHECK(algo IN ("CGNR", "CGNE")),
  start_date          TEXT NOT NULL,
  end_date            TEXT NOT NULL,
  data                TEXT NOT NULL,  -- Trocar isso para blob talvez
  height              INTEGER NOT NULL,
  width               INTEGER NOT NULL,
  iterations          INTEGER NOT NULL,
  reconstruction_time INTEGER NOT NULL,
  user                INTEGER NOT NULL,
  FOREIGN KEY(user) REFERENCES users(id)
);