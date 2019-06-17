-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- proc_get_occbool_bimmodels_calc
-- 获取 OCCBOOL 算量用 构件情报信息
-- ############################################################################

USE zer;

DROP PROCEDURE IF EXISTS proc_get_occbool_bimmodels_calc;
DELIMITER ;;
CREATE PROCEDURE proc_get_occbool_bimmodels_calc(
  IN model_db_name VARCHAR(64)   -- 构件 schema 名称
  , IN category_ids VARCHAR(128) -- 算量用的构件类别Category组
                                 -- 数据来源参照 hg::bim::Category
                                 -- 格式: Category|Category|Category...
  , IN fetch_all INTEGER         -- 是否全取数据
                                 -- 非0: 全取
                                 -- 第一次取 构件情报信息可能带过滤条件
                                 -- 之后需要全取
)
sp_main: BEGIN
  -- Routine body goes here...
  DECLARE exists_model_db VARCHAR(64);
  DECLARE instance_name VARCHAR(48);
  DECLARE bim_category INTEGER;
  DECLARE fetch_occbool_bimmodels_SQL VARCHAR(1024);
  DECLARE max_count_category INTEGER DEFAULT 100;
  DECLARE max_count_category_opt VARCHAR(64);

  -- 遍历数据结束标志
  DECLARE done INT DEFAULT FALSE;
  -- 游标
  DECLARE cur CURSOR FOR SELECT InstanceName
    FROM TEMP_TB_OCCBOOL_CALC_INSTANCE_INFO;
  DECLARE cur_cat CURSOR FOR SELECT Category
    FROM TEMP_TB_OCCBOOL_CALC_CATEGORY_INFO;
  -- 将结束标志绑定到游标
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

  SET exists_model_db
    = (SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA
         WHERE SCHEMA_NAME = model_db_name);
  IF (ISNULL(exists_model_db)) THEN
    CALL proc_get_results_1st(1
      -- , CONCAT('ERROR:  ', model_db_name, ' NOT EXISTS')
      , CONCAT('ERROR: 指定数据库 ', model_db_name, ' 不存在')
      , @erro_no);
    IF (@erro_no <> 0) THEN
      LEAVE sp_main;
    END IF;
  END IF;

  CREATE TEMPORARY TABLE IF NOT EXISTS TEMP_TB_OCCBOOL_CALC_INSTANCE_INFO (
    InstanceName VARCHAR(48)
  ) ENGINE = MEMORY;

  CREATE TEMPORARY TABLE IF NOT EXISTS TEMP_TB_OCCBOOL_CALC_CATEGORY_INFO (
    Category INTEGER
  ) ENGINE = MEMORY;

  CREATE TEMPORARY TABLE IF NOT EXISTS TEMP_TB_OCCBOOL_CALC_MODEL_INFO (
    IDinModel VARCHAR(48)
    , Category INTEGER
    , ElementID INTEGER
    , Name VARCHAR(128)
    , SumCost_EX DOUBLE DEFAULT 0.00
    , SelfCost_EX DOUBLE DEFAULT 0.00
    , Expression_EX TEXT DEFAULT NULL
  ); --  ENGINE = MEMORY 不能应用此类型, 内存类型表类型不支持BLOB/TEXT列
     --  Error Code: 1163. The used table type doesn't support BLOB/TEXT columns
     
  CREATE TEMPORARY TABLE IF NOT EXISTS TEMP_TB_TODO_OCCBOOL_CALC_MODEL_INFO (
    SELECT * FROM TEMP_TB_OCCBOOL_CALC_MODEL_INFO WHERE 0 != 0
  );

  TRUNCATE TABLE TEMP_TB_OCCBOOL_CALC_INSTANCE_INFO;
  TRUNCATE TEMP_TB_OCCBOOL_CALC_CATEGORY_INFO;
  TRUNCATE TABLE TEMP_TB_OCCBOOL_CALC_MODEL_INFO;
  TRUNCATE TABLE TEMP_TB_TODO_OCCBOOL_CALC_MODEL_INFO;

  INSERT INTO TEMP_TB_OCCBOOL_CALC_INSTANCE_INFO
    VALUES('BuildingInstance')  -- 基本构件
    , ('DecorateInstance')      -- 装饰构件
    , ('WaterProofInstance')    -- 防水构件
  ;

  -- reset category_ids
  SET category_ids = REPLACE(category_ids, ' ', '');
  SET category_ids = REPLACE(category_ids, '|', ',');

  -- 打开游标
  OPEN cur;

  -- 开始循环
  read_loop: LOOP
    -- 提取游标里的数据
    FETCH cur INTO instance_name;

    -- 声明结束的时候
    IF done THEN
      LEAVE read_loop;
    END IF;

    -- 这里做你想做的循环的事件
    SET @category_ids = category_ids;
    SET fetch_occbool_bimmodels_SQL = CONCAT('
    INSERT INTO TEMP_TB_OCCBOOL_CALC_MODEL_INFO
      SELECT B.IDinModel
        , B.Category
        , B.ElementID
        , B.Name
        , C.SumCost
        , C.SelfCost
        , C.Expression
      FROM `'
      , model_db_name
      , '`.'
      , instance_name
      , ' B LEFT JOIN hgdataserver.`'
      , model_db_name
      , '` C'
      , ' ON (B.IDinModel = C.ModelID)'
      , ' WHERE FIND_IN_SET(B.Category, @category_ids)'
      ,';');
    SELECT fetch_occbool_bimmodels_SQL INTO @fetch_occbool_bimmodels_SQL;
    BEGIN
      PREPARE fetch_occbool_bimmodels_SQL FROM @fetch_occbool_bimmodels_SQL;
      EXECUTE fetch_occbool_bimmodels_SQL;
    END;
  END LOOP;

  -- 关闭游标
  CLOSE cur;

  CALL proc_get_results_1st(0, 'OK', @erro_no);
  IF (@erro_no <> 0) THEN
    LEAVE sp_main;
  END IF;

  SELECT O.value INTO max_count_category_opt
    FROM tbl_options O
    WHERE O.code = '1D8AF05B-6748-11E9-BC87-0242AC120004'
  ;
  IF (max_count_category_opt IS NOT NULL) THEN
    SET max_count_category = CAST(max_count_category_opt AS UNSIGNED INTEGER);
  END IF;

  IF (max_count_category = 0 OR fetch_all != 0) THEN
    SELECT * FROM TEMP_TB_OCCBOOL_CALC_MODEL_INFO T21
      ORDER BY T21.Category, T21.ElementID, T21.Name
    ;
  ELSE
    BEGIN
      INSERT INTO TEMP_TB_OCCBOOL_CALC_CATEGORY_INFO
        SELECT T21.Category FROM TEMP_TB_OCCBOOL_CALC_MODEL_INFO T21
        GROUP BY T21.Category;

      SET done = FALSE;
        -- 打开游标
      OPEN cur_cat;

      -- 开始循环
      read_loop: LOOP
        -- 提取游标里的数据
        FETCH cur_cat INTO bim_category;

        -- 声明结束的时候
        IF done THEN
          LEAVE read_loop;
        END IF;

        -- 这里做你想做的循环的事件
        SET fetch_occbool_bimmodels_SQL = CONCAT(
          'INSERT INTO TEMP_TB_TODO_OCCBOOL_CALC_MODEL_INFO'
          , ' SELECT * FROM TEMP_TB_OCCBOOL_CALC_MODEL_INFO T21'
          , ' WHERE T21.Category = '
          , bim_category
          , ' ORDER BY T21.Category, T21.ElementID, T21.Name LIMIT '
          , max_count_category_opt
          , ';');
        SELECT fetch_occbool_bimmodels_SQL INTO @fetch_occbool_bimmodels_SQL;
        BEGIN
          PREPARE fetch_occbool_bimmodels_SQL FROM @fetch_occbool_bimmodels_SQL;
          EXECUTE fetch_occbool_bimmodels_SQL;
        END;

      END LOOP;

      -- 关闭游标
      CLOSE cur_cat;

      SELECT * FROM TEMP_TB_TODO_OCCBOOL_CALC_MODEL_INFO T21
        ORDER BY T21.Category, T21.ElementID, T21.Name
      ;
    END;
  END IF;

  DROP TABLE TEMP_TB_TODO_OCCBOOL_CALC_MODEL_INFO;
  DROP TABLE TEMP_TB_OCCBOOL_CALC_MODEL_INFO;
  DROP TABLE TEMP_TB_OCCBOOL_CALC_CATEGORY_INFO;
  DROP TABLE TEMP_TB_OCCBOOL_CALC_INSTANCE_INFO;
END
;;
DELIMITER ;

SELECT '  创建 获取 OCCBOOL 算量用 构件情报信息(proc_get_occbool_bimmodels_calc)' AS '成功:';

-- CALL proc_get_occbool_bimmodels_calc('9caf303f-66cb-4c53-95ea-db84fe3032c5', '1|3|5|7');
