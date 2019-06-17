-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- proc_get_results_1st
-- 执行业务存储过程返回的第一数据集（状态信息）
-- ############################################################################

USE zer;

DROP PROCEDURE IF EXISTS proc_get_results_1st;
DELIMITER ;;
CREATE PROCEDURE proc_get_results_1st(IN erro_no INTEGER
  , IN erro_message VARCHAR(256)
  , OUT result INTEGER
)
BEGIN
  -- Routine body goes here...
  SET result = erro_no;
  SELECT erro_no AS 'erro_no'
    , erro_message AS 'erro_message'
  ;
END
;;
DELIMITER ;

SELECT '  创建 执行业务存储过程返回的第一数据集（状态信息）(proc_get_results_1st)'
  AS '成功:';

-- CALL proc_get_results_1st(0, 'OK', @result);