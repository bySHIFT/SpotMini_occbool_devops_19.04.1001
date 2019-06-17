-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- 删除 数据库(zer)
-- 慎用
DROP DATABASE IF EXISTS zer;
SELECT '  删除 数据库(zer)' AS '成功:';

-- 刷新系统权限表
FLUSH PRIVILEGES;