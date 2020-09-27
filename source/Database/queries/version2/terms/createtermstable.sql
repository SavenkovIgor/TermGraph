CREATE TABLE terms (
    uuid        TEXT PRIMARY KEY
                     NOT NULL,
    term        TEXT NOT NULL,
    definition  TEXT,
    description TEXT,
    examples    TEXT,
    wikiUrl     TEXT,
    wikiImage   TEXT,
    groupUuid   TEXT NOT NULL,
    lastEdit    TEXT NOT NULL
);
