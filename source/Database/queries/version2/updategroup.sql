UPDATE groups
   SET name = :name,
       comment = :comment
 WHERE uuid = :uuid;
