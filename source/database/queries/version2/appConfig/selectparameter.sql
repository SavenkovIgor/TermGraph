SELECT value
  FROM appConfig
 WHERE parameter = :parameter
 LIMIT 1;
