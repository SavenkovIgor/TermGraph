INSERT INTO terms (
                      uuid,
                      term,
                      definition,
                      description,
                      examples,
                      wikiUrl,
                      wikiImage,
                      groupUuid,
                      lastEdit
                  )
                  VALUES (
                      :uuid,
                      :term,
                      :definition,
                      :description,
                      :examples,
                      :wikiUrl,
                      :wikiImage,
                      :groupUuid,
                      :lastEdit
                  );
