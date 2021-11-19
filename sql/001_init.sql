CREATE TABLE Images (
  username            TEXT PRIMARY KEY,
  algo                TEXT CHECK(algo IN ("CGNR", "CGNE")),
  start_date          TEXT,
  end_date            TEXT,
  data                TEXT,  -- Trocar isso para blob talvez
  height              INTEGER,
  width               INTEGER,
  iterations          INTEGER,
  reconstruction_time INTEGER
);