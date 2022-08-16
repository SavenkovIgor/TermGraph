SELECT G.uuid,
       G.name,
       G.comment,
       count(T.groupUuid) AS size
  FROM groups AS G LEFT OUTER JOIN terms AS T ON G.uuid = T.groupUuid
GROUP BY ifnull(T.groupUuid, G.uuid);
