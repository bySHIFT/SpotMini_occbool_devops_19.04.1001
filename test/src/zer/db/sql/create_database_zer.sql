-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- 创建 数据库(zer)
CREATE DATABASE IF NOT EXISTS zer CHARACTER SET UTF8;
SELECT '  创建 数据库(zer)' AS '成功:';

-- 刷新系统权限表
FLUSH PRIVILEGES;