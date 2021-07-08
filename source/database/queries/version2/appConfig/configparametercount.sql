SELECT COUNT( * ) 
  FROM appConfig
 WHERE parameter = :parameter
 LIMIT 1;
