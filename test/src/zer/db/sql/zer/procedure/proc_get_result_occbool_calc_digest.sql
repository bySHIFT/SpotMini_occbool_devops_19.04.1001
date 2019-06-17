-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

USE zer;

-- ############################################################################
-- proc_get_result_occbool_calc_digest
-- 获取 内部用OCCBOOL 算量后计算结果的分析摘要信息
-- ############################################################################
DROP PROCEDURE IF EXISTS proc_get_result_occbool_calc_digest_private;
DELIMITER ;;
CREATE PROCEDURE proc_get_result_occbool_calc_digest_private(
  IN tips VARCHAR(64)     -- 错误分类名称
  , OUT DIGEST MEDIUMTEXT -- 明细
)
BEGIN
  -- Routine body goes here...
  -- 临时表字段
  DECLARE tmp_tb_idx INTEGER;
  DECLARE tmp_tb_bimmodel_RecordID INTEGER;
  DECLARE tmp_tb_project_Name VARCHAR(64);
  DECLARE tmp_tb_project_Type VARCHAR(64);
  DECLARE tmp_tb_project_BimCodeTable VARCHAR(64);
  DECLARE tmp_tb_bimmodel_ModelDBName VARCHAR(64);
  DECLARE tmp_tb_bimmodel_VersionName VARCHAR(64);
  DECLARE tmp_tb_bimmodel_CreateTime VARCHAR(64);
  DECLARE tmp_tb_bimmodel_CalcRule VARCHAR(64);
  DECLARE tmp_tb_bim_IDinModel VARCHAR(64);
  DECLARE tmp_tb_bim_Category INTEGER;
  DECLARE tmp_tb_bim_ElementID INTEGER;
  DECLARE tmp_tb_bim_Name VARCHAR(128);
  DECLARE tmp_tb_bim_SumCost_EX DOUBLE;
  DECLARE tmp_tb_bim_SelfCost_EX DOUBLE;
  DECLARE tmp_tb_bim_Expression_EX TEXT;
  DECLARE tmp_tb_bim_SumCost DOUBLE;
  DECLARE tmp_tb_bim_SelfCost DOUBLE;
  DECLARE tmp_tb_bim_Expression TEXT;
  DECLARE tmp_tb_devops_STATE INTEGER;
  DECLARE tmp_tb_devops_CreateTime VARCHAR(64);

  DECLARE tmp_DIGEST MEDIUMTEXT;

  -- 遍历数据结束标志
  DECLARE done INT DEFAULT FALSE;
  -- 游标
  DECLARE cur CURSOR FOR SELECT idx
    , bimmodel_RecordID 
    , project_Name
    , project_Type
    , project_BimCodeTable
    , bimmodel_ModelDBName
    , bimmodel_VersionName
    , bimmodel_CreateTime
    , bimmodel_CalcRule
    , bim_IDinModel
    , bim_Category
    , bim_ElementID
    , bim_Name
    , bim_SumCost_EX
    , bim_SelfCost_EX
    , bim_Expression_EX
    , bim_SumCost
    , bim_SelfCost
    , bim_Expression
    , devops_STATE
    , devops_CreateTime
  FROM TEMP_TB_1555492883__000016202668630556518816;
  -- 将结束标志绑定到游标
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  -- CHAR(10): 换行符
  -- CHAR(13): 回车符

  SET @t_idx = 0;
  SET DIGEST = '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>';

  -- 打开游标
  OPEN cur;
  -- 开始循环
  read_loop: LOOP
  -- 提取游标里的数据
    FETCH cur INTO tmp_tb_idx
      , tmp_tb_bimmodel_RecordID 
      , tmp_tb_project_Name
      , tmp_tb_project_Type
      , tmp_tb_project_BimCodeTable
      , tmp_tb_bimmodel_ModelDBName
      , tmp_tb_bimmodel_VersionName
      , tmp_tb_bimmodel_CreateTime
      , tmp_tb_bimmodel_CalcRule
      , tmp_tb_bim_IDinModel
      , tmp_tb_bim_Category
      , tmp_tb_bim_ElementID
      , tmp_tb_bim_Name
      , tmp_tb_bim_SumCost_EX
      , tmp_tb_bim_SelfCost_EX
      , tmp_tb_bim_Expression_EX
      , tmp_tb_bim_SumCost
      , tmp_tb_bim_SelfCost
      , tmp_tb_bim_Expression
      , tmp_tb_devops_STATE
      , tmp_tb_devops_CreateTime
    ;

    -- 声明结束的时候
    IF done THEN
      LEAVE read_loop;
    END IF;

    -- 这里做你想做的循环的事件
    -- 可能存在错误构件过多(超百条)
    -- 如果全部输出, 影响用户体验(MySQL 组织这些数据也很耗时)
    -- 所以这里只输出部分错误构件信息
    -- IF @t_idx >= 20 THEN
    --   SET DIGEST = CONCAT(DIGEST
    --     , CHAR(10)
    --     , '---------- MORE AND MORE ----------');

    --   LEAVE read_loop;
    -- END IF;

    SET @t_idx = @t_idx + 1;
    SET tmp_DIGEST = '';
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  [', tips, '>> ', @t_idx, '/', @t_cnt, ']');
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  记录ID: ', tmp_tb_bimmodel_RecordID);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  工程名称: ', tmp_tb_project_Name);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  工程类型: ', tmp_tb_project_Type);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  工程用BIMCODE表: ', tmp_tb_project_BimCodeTable);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  构件库名称: ', tmp_tb_bimmodel_ModelDBName);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  版本: ', tmp_tb_bimmodel_VersionName);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  创建日期: ', tmp_tb_bimmodel_CreateTime);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '  计算规则: ', tmp_tb_bimmodel_CalcRule);

    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件UUID: ', tmp_tb_bim_IDinModel);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件类别: ', tmp_tb_bim_Category);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件ElementID: ', tmp_tb_bim_ElementID);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件名称: ', tmp_tb_bim_Name);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件量(测试前): ', tmp_tb_bim_SumCost_EX);
    SET tmp_DIGEST = CONCAT(tmp_DIGEST, CHAR(10), '    构件量(测试后): '
      , IF(tmp_tb_devops_STATE = 15, tmp_tb_bim_SumCost, 'N/A'));

    SET DIGEST = CONCAT(DIGEST, tmp_DIGEST);
  END LOOP;

  -- 关闭游标
  CLOSE cur;

  SET DIGEST = CONCAT(DIGEST
    , CHAR(10)
    , '<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<');
END
;;
DELIMITER ;

SELECT '  创建 内部用获取 OCCBOOL 算量后计算结果的分析摘要信息(proc_get_result_occbool_calc_digest_private)' AS '成功:';

-- ############################################################################
-- proc_get_result_occbool_calc_digest
-- 获取 OCCBOOL 算量后计算结果的分析摘要信息
-- ############################################################################
DROP PROCEDURE IF EXISTS proc_get_result_occbool_calc_digest;
DELIMITER ;;
CREATE PROCEDURE proc_get_result_occbool_calc_digest(
  IN result_tbl_name VARCHAR(64)   -- 构件 schema 名称
)
sp_main: BEGIN
  -- Routine body goes here...
  DECLARE prepared_cmd VARCHAR(1024);
  DECLARE ERROR_NO INTEGER DEFAULT 0;
  DECLARE DIGEST MEDIUMTEXT;
  DECLARE total_table INTEGER;
  DECLARE tolerance DOUBLE DEFAULT 0.005;
  DECLARE tolerance_opt VARCHAR(64);

  SELECT O.value INTO tolerance_opt
    FROM tbl_options O
    WHERE O.code = 'E521F9DE-60EA-11E9-BC87-0242AC120004'
  ;
  IF (tolerance_opt IS NOT NULL) THEN
    SET tolerance = CAST(tolerance_opt AS DECIMAL(8, 4));
  END IF;

  CALL proc_get_results_1st(0, 'OK', @erro_no);
  IF (@erro_no <> 0) THEN
    LEAVE sp_main;
  END IF;

  -- CHAR(10): 换行符
  -- CHAR(13): 回车符
  loop_label: LOOP
    SET DIGEST = '>> ';
    SET DIGEST = CONCAT(DIGEST, CURRENT_TIMESTAMP);
    SET ERROR_NO = 0;

    -- 临时表的创建
    SET prepared_cmd = CONCAT('CREATE TEMPORARY TABLE IF NOT EXISTS'
      , ' TEMP_TB_1555492883__000016202668630556518816'
      , ' (SELECT * FROM '
      , result_tbl_name
      , ' WHERE 0 != 0);');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    TRUNCATE TABLE TEMP_TB_1555492883__000016202668630556518816;

    -- 总算量构件数
    SET prepared_cmd = CONCAT('SET @t_cnt = (SELECT COUNT(*) FROM '
      , result_tbl_name, ');');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    IF ((@t_cnt IS NULL) OR (@t_cnt <= 0)) THEN
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '待分析算量结果节为空');

      LEAVE loop_label;
    ELSE
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '总算量构件数: ', @t_cnt);
    END IF;

    -- 算量构件数(未计算|计算不成功)
    -- devops_STATE
    -- enum STATE {
    --   TODO = 1
    --   , SEND = TODO + (1 << 1)
    --   , ACKED = SEND + (1 << 2)
    --   , SUCCESS_CALC = ACKED + (1 << 3)
    -- };
    TRUNCATE TABLE TEMP_TB_1555492883__000016202668630556518816;
    SET prepared_cmd = CONCAT('INSERT INTO TEMP_TB_1555492883__000016202668630556518816'
      , ' (SELECT * FROM ', result_tbl_name
      , ' T21 WHERE T21.devops_STATE != 15);');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    SET @t_cnt = (SELECT COUNT(*) FROM TEMP_TB_1555492883__000016202668630556518816);
    IF ((@t_cnt IS NULL) OR (@t_cnt <= 0)) THEN
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(未计算|计算不成功): 0');
    ELSE
      SET ERROR_NO = 9999;
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(未计算|计算不成功): ', @t_cnt);

      CALL proc_get_result_occbool_calc_digest_private('未计算|计算不成功', @DIGEST);
      SET DIGEST = CONCAT(DIGEST, CHAR(10), @DIGEST);
     END IF;

    -- 算量构件数(负数)
    TRUNCATE TABLE TEMP_TB_1555492883__000016202668630556518816;
    SET prepared_cmd = CONCAT('INSERT INTO TEMP_TB_1555492883__000016202668630556518816'
      , ' (SELECT * FROM ', result_tbl_name
      , ' T21 WHERE T21.bim_SumCost < 0'
      , ' AND T21.devops_STATE = 15'
      , ' );');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    SET @t_cnt = (SELECT COUNT(*) FROM TEMP_TB_1555492883__000016202668630556518816);
    IF ((@t_cnt IS NULL) OR (@t_cnt <= 0)) THEN
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(负数): 0');
    ELSE
      SET ERROR_NO = 9999;
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(负数): ', @t_cnt);

      CALL proc_get_result_occbool_calc_digest_private('负数', @DIGEST);
      SET DIGEST = CONCAT(DIGEST, CHAR(10), @DIGEST);
    END IF;

    -- 算量构件数(新增计算)
    TRUNCATE TABLE TEMP_TB_1555492883__000016202668630556518816;
    SET prepared_cmd = CONCAT('INSERT INTO TEMP_TB_1555492883__000016202668630556518816'
      , ' (SELECT * FROM ', result_tbl_name
      , ' T21 WHERE T21.bim_SumCost_EX = 0'
      , ' AND T21.bim_SelfCost_EX = 0'
      , ' AND CHAR_LENGTH(T21.bim_Expression_EX) = 0'
      , ' AND T21.devops_STATE = 15'
      , ');');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    SET @t_cnt = (SELECT COUNT(*) FROM TEMP_TB_1555492883__000016202668630556518816);
    IF ((@t_cnt IS NULL) OR (@t_cnt <= 0)) THEN
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(新增计算): 0');
    ELSE
      -- SET ERROR_NO = 9999; -- 新追加计算的构件, 不认为是错误
                              -- 如果是出现负值, 那么已经在上一个检查项里有体现
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(新增计算): ', @t_cnt);

      CALL proc_get_result_occbool_calc_digest_private('新增计算', @DIGEST);
      SET DIGEST = CONCAT(DIGEST, CHAR(10), @DIGEST);
    END IF;

    -- 算量构件数(公差外)
    TRUNCATE TABLE TEMP_TB_1555492883__000016202668630556518816;
    SET prepared_cmd = CONCAT('INSERT INTO TEMP_TB_1555492883__000016202668630556518816'
      , ' (SELECT * FROM ', result_tbl_name
      , ' T21 WHERE CHAR_LENGTH(T21.bim_Expression_EX) > 0'
      , ' AND T21.bim_SumCost_EX >= 0 AND T21.bim_SumCost >= 0'
-- 公差为值的比较
--      , ' AND ABS(T21.bim_SumCost_EX - T21.bim_SumCost) > '
-- END
-- 公差为比例的比较
      , ' AND T21.bim_SumCost_EX > 0 AND ABS(T21.bim_SumCost_EX - T21.bim_SumCost)'
      , ' / T21.bim_SumCost_EX > '
-- END
      , tolerance
      , ' AND T21.devops_STATE = 15'
      , ');');
    SELECT prepared_cmd INTO @PREPARED_CMD;
    BEGIN
      PREPARE prepared_cmd FROM @PREPARED_CMD;
      EXECUTE prepared_cmd;
    END;

    SET @t_cnt = (SELECT COUNT(*) FROM TEMP_TB_1555492883__000016202668630556518816);
    IF ((@t_cnt IS NULL) OR (@t_cnt <= 0)) THEN
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(公差[', tolerance, ']外): 0');
    ELSE
      SET ERROR_NO = 9999;
      SET DIGEST = CONCAT(DIGEST, CHAR(10), '算量构件数(公差[', tolerance, ']外): ', @t_cnt);

      CALL proc_get_result_occbool_calc_digest_private(CONCAT('公差[', tolerance, ']外'), @DIGEST);
      SET DIGEST = CONCAT(DIGEST, CHAR(10), @DIGEST);
    END IF;

    -- ################################################## END
    LEAVE loop_label;
  END LOOP;

  SELECT ERROR_NO AS 'erro_no'
    , DIGEST AS 'digest'
  ;

  DROP TABLE TEMP_TB_1555492883__000016202668630556518816;
END
;;
DELIMITER ;

SELECT '  创建 获取 OCCBOOL 算量后计算结果的分析摘要信息(proc_get_result_occbool_calc_digest)' AS '成功:';

-- CALL proc_get_result_occbool_calc_digest('tbl_occbool_devops_result__1555500773__000014957654104455017837');
