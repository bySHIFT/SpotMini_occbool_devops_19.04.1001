-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- 初始化 zer 数据库
-- ############################################################################

SET NAMES utf8;

-- 测试用, 慎用
source ./drop_database_zer.sql

source ./create_database_zer.sql
source ./zer/table/tbl_options.sql
source ./zer/table/tbl_devops_history.sql
source ./zer/procedure/proc_get_results_1st.sql
source ./zer/procedure/proc_get_db_version.sql
source ./zer/procedure/proc_get_biz_options.sql
source ./zer/procedure/proc_get_occbool_projects_calc.sql
source ./zer/procedure/proc_get_occbool_bimmodels_calc.sql
source ./zer/procedure/proc_get_result_occbool_calc_digest.sql
source ./zer/init_tbl_options.sql

-- 追加 OCCBOOL 算量用构件相关数据
-- 存在一定风险, 慎用

-- gw 3227 工业五路站
-- 1.5.6测试版--代
-- 工程信息: 工业五1.5.6 主体
-- 模型信息: 3227 505888cd-f83d-4c11-a741-9d3e588ea109
-- 计算规则: 轨道交通BIM分类清单计算规则20190529 e4a16c25-ef8f-4c3a-8aca-a271a538562b
-- 算量总构件数: 1475
-- 估计用时(单位分钟)： 63.5833
-- 适用 release-v1.5   09ebf891

-- wh 3229 文化路站
-- 陆1.5.06.0530正在测试
-- 工程信息: 文化站3 1主体
-- 模型信息: 3229 38bf5ee0-be4f-4967-bedb-0f2355d0968c
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 821
-- 估计用时(单位分钟)： 49.7833
-- 适用 release-v1.5   09ebf891

-- bs 3237 北沙滩站
-- 1.5.6测试版--代
-- 工程信息: 北沙滩站1.5.6 主体
-- 模型信息: 3237 7adda195-b4f2-4748-95da-0b42212a4048
-- 计算规则: 轨道交通BIM分类清单计算规则20190529 e4a16c25-ef8f-4c3a-8aca-a271a538562b
-- 算量总构件数: 819
-- 估计用时(单位分钟)： 29.5
-- 适用 release-v1.5   09ebf891

-- ws 3243 万寿路站
-- 1.5.6.0530测试文件薛
-- 工程信息: 万寿路1.5.6 2
-- 模型信息: 3243 5a8e4ee8-a063-4998-a0d9-b253afd78a55
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 1637
-- 估计用时(单位分钟)： 69.2
-- 适用 release-v1.5   09ebf891

-- wp 3250 宛平站
-- 1.5.6测试版--代
-- 工程信息: 宛平路1.5.6 主体
-- 模型信息: 3250 9d443248-87ae-40a4-a544-4bdd8fe502b7
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 704
-- 估计用时(单位分钟)： 25.9167
-- 适用 release-v1.5   09ebf891

-- sk 3251 沙口路站
-- 1.5.6.0530测试文件薛
-- 工程信息: 沙口路1.5.6 1
-- 模型信息: 3251 46f9f95a-d388-4328-b480-04a5c0312331
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 1787
-- 估计用时(单位分钟)： 69.75
-- 适用 release-v1.5   09ebf891

-- fy 3265 丰益桥
-- 1.5.6.0530测试文件薛
-- 工程信息: 丰益桥防水测试_0505 6
-- 模型信息: 3265 769a8a14-9998-43d6-bf05-acb5473c8f69
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 825
-- 估计用时(单位分钟)： 28.8667
-- 适用 release-v1.5   09ebf891

-- lq 3267 芦岐站
-- 版本验证0505
-- 工程信息: 芦岐站0531 1
-- 模型信息: 3267 fabeb69c-a5e2-4881-b71b-4172d43f1935
-- 计算规则: 轨道交通BIM分类清单计算规则201904024 d1b09da6-dc90-4433-9481-67f3cebdd640
-- 算量总构件数: 596
-- 估计用时(单位分钟)： 20.4167
-- 适用 master   0328ac5e 2019/06/12

source ./data/dumps/dump20190605_3227_3229_3237_3243_3250_3251_3265_3267.sql
source ./data/dumps/dump20190605_hgdataserver_3227_3229_3237_3243_3250_3251_3265_3267.sql
