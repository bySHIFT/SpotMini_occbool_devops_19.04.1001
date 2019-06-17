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

-- 工程信息: 磁各庄AB A出入口
-- 适用 OCCBOOL REALEASE-V1.3
-- source ./data/dumps/dump20190424_2362_67ed700b-d879-4129-b52b-b68fffaf4093.sql
-- source ./data/dumps/dump20190424_2362_hgdataserver.sql

-- 工程信息: 沙口 1
-- 适用 OCCBOOL REALEASE-V1.4
-- source ./data/dumps/dump20190424_2723_c3c8f3c2-7f5f-4d9e-a54b-94c4edbbee79.sql
-- source ./data/dumps/dump20190424_2723_hgdataserver.sql

-- 1.4.6.0415测试文件
-- 工程信息: 万寿路2 1
-- 适用 OCCBOOL REALEASE-V1.4
-- source ./data/dumps/dump20190425_2652_6d397048-a188-472d-be5c-d905e203b53f.sql
-- source ./data/dumps/dump20190425_2652_hgdataserver.sql

-- 此工程不可参照
--   导出源的参照量是在2个计算规则下计算的
--   参照量是用的 轨道交通BIM分类清单计算规则201904018
--   新计算用的是 轨道交通BIM分类清单计算规则201904024
-- 1.4.11.0425测试文件
-- 工程信息: 芦岐路0425 1
-- 适用 OCCBOOL REALEASE-V1.4
-- source ./data/dumps/dump20190426_2758_43212367-3ea4-4ac4-86a4-c8037c260c65.sql
-- source ./data/dumps/dump20190426_2758_hgdataserver.sql

-- 此工程不可参照
--   有大部分构件的BIMCODE未挂接
-- 1.4.11.0425测试文件
-- 工程信息: 济南备用中心0425 1
-- 计算规则: 轨道交通BIM分类清单计算规则201904024
-- 适用 OCCBOOL REALEASE-V1.4
-- source ./data/dumps/dump20190426_2770_7f60bf78-e297-4c9f-a18b-b5057482478c.sql
-- source ./data/dumps/dump20190426_2770_hgdataserver.sql

-- 1.4.6.0415测试文件
-- 工程信息: 宛平路2 1
-- 计算规则: 轨道交通BIM分类清单计算规则20190408
-- 适用 OCCBOOL REALEASE-V1.4
source ./data/dumps/dump20190426_2643_96740f24-7332-4399-96ca-83f7206a643f.sql
source ./data/dumps/dump20190426_2643_hgdataserver.sql
