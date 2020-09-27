CREATE TABLE groups (
    uuid    TEXT PRIMARY KEY
                 NOT NULL,
    name    TEXT UNIQUE
                 NOT NULL,
    comment TEXT
);
