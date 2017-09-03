CREATE TABLE IF NOT EXISTS records(
    fileName    TEXT PRIMARY KEY NOT NULL,
    project     TEXT,
    designType  TEXT,
    tokens      TEXT
);