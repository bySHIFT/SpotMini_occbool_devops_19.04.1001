-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ##################################################
-- proc_get_biz_options
-- 获取 应用相关设定配置信息
-- ##################################################

USE zer;

DROP PROCEDURE IF EXISTS proc_get_biz_options;
DELIMITER ;;
CREATE PROCEDURE proc_get_biz_options()
sp_main: BEGIN
  -- Routine body goes here...
  CALL proc_get_results_1st(0, 'OK', @erro_no);
  IF (@erro_no <> 0) THEN
    LEAVE sp_main;
  END IF;

  SELECT O.code   -- 应用相关的代码(唯一标识)
    , O.value     -- 值
    , O.comment   -- 概要
    FROM tbl_options O
    ORDER BY O.idx
  ;

END
;;
DELIMITER ;

SELECT '  创建 获取 应用相关设定配置信息(proc_get_biz_options)'
  AS '成功:';

-- CALL proc_get_biz_options();