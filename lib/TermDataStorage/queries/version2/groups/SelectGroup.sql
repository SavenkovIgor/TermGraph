  SELECT G.uuid,
         G.name,
         G.comment,
         count(T.groupUuid) AS size
    FROM groups AS G LEFT OUTER JOIN terms AS T ON G.uuid = T.groupUuid
   WHERE G.uuid = :uuid
GROUP BY T.groupUuid;
