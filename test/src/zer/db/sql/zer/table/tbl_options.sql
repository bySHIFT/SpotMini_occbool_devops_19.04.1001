-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- tbl_options
-- zer 数据库 应用相关设定表
-- ############################################################################

USE zer;

-- ############################################################################
-- tbl_options: 应用相关设定表
-- ############################################################################
CREATE TABLE IF NOT EXISTS tbl_options
(
  idx INTEGER UNSIGNED PRIMARY KEY AUTO_INCREMENT -- 顺序号
  , code VARCHAR(36) NOT NULL       -- 应用相关的代码(唯一标识)
  , value VARCHAR(128) NOT NULL     -- 值
  , comment VARCHAR(256) NOT NULL   -- 概要

  , UNIQUE(code)
);
SELECT '  创建 tbl_options(应用相关设定表)' AS '成功:';