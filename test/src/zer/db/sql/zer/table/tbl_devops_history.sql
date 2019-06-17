-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- tbl_devops_history
-- zer 测试历史记录表
-- ############################################################################

USE zer;

-- ############################################################################
-- tbl_devops_history: 测试历史记录表
-- ############################################################################
CREATE TABLE IF NOT EXISTS tbl_devops_history
(
  idx INTEGER UNSIGNED PRIMARY KEY AUTO_INCREMENT -- 顺序号
  , result_calc_tbl_name VARCHAR(64) NOT NULL     -- 存储计算结果内容的表名称
  , comment VARCHAR(256) DEFAULT NULL             -- 概要
  , register_timestamp TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
);
SELECT '  创建 tbl_devops_history(测试历史记录表)' AS '成功:';