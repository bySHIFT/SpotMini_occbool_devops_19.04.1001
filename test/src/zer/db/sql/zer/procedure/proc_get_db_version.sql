-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- proc_get_db_version
-- 获取 zer 数据库版本信息
-- ############################################################################

USE zer;

DROP PROCEDURE IF EXISTS proc_get_db_version;
DELIMITER ;;
CREATE PROCEDURE proc_get_db_version()
sp_main: BEGIN
  -- Routine body goes here...
  DECLARE major INTEGER DEFAULT 0;
  DECLARE minor INTEGER DEFAULT 0;
  DECLARE rev INTEGER DEFAULT 0;
  DECLARE db_version VARCHAR(32);

  SELECT O.value INTO db_version
    FROM tbl_options O
    WHERE O.code = '38D4F118-5A8F-11E9-BC87-0242AC120004'
  ;

  CALL proc_get_results_1st(0, 'OK', @erro_no);
  IF (@erro_no <> 0) THEN
    LEAVE sp_main;
  END IF;

  SET major = CONVERT(SUBSTRING_INDEX(db_version, '.', 1), UNSIGNED INTEGER);
  SET minor = CONVERT(SUBSTRING_INDEX(SUBSTRING_INDEX(db_version, '.', 2)
    , '.', -1), UNSIGNED INTEGER);
  SET rev = CONVERT(SUBSTRING_INDEX(db_version, '.', -1), UNSIGNED INTEGER);

  SELECT CONCAT(major
        , '.', RIGHT(CONCAT(REPEAT('0', 2), minor), 2)
        , '.', RIGHT(CONCAT(REPEAT('0', 6), rev), 6)) AS 'db_version'
    , major AS 'db_version_major'
    , minor AS 'db_version_minor'
    , rev AS 'db_version_revision'
  ;
END
;;
DELIMITER ;

SELECT '  创建 获取 zer 数据库版本信息(proc_get_db_version)' AS '成功:';

-- CALL proc_get_db_version();