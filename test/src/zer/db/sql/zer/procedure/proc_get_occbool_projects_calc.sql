-- NOTICE
--   Copyright 2019 bySHIFT
--   All rights reserved

-- ############################################################################
-- proc_get_occbool_projects_calc
-- 获取 OCCBOOL 算量用 工程情报信息
-- ############################################################################

USE zer;

DROP PROCEDURE IF EXISTS proc_get_occbool_projects_calc;
DELIMITER ;;
CREATE PROCEDURE proc_get_occbool_projects_calc(
  IN record_ids VARCHAR(128) -- 查询参照用的RecordID组
                             -- 数据来源 hgdataserver.BIMMODEL.RecordID
                             -- 格式: RecordID|RecordID|RecordID...
)
sp_main: BEGIN
  -- Routine body goes here...

  -- reset record_ids
  SET record_ids = REPLACE(record_ids, ' ', '');
  SET record_ids = REPLACE(record_ids, '|', ',');

  CALL proc_get_results_1st(0, 'OK', @erro_no);
  IF (@erro_no <> 0) THEN
    LEAVE sp_main;
  END IF;

  SELECT B.RecordID AS bimmodel_RecordID
    , CONCAT(T.Name, '_', P.Name) AS project_Name
    , P.Type AS project_Type
    , P.BimCodeTable AS project_BimCodeTable
    , B.ModelDBName AS bimmodel_ModelDBName
    , B.VersionName AS bimmodel_VersionName
    , B.CreateTime AS bimmodel_CreateTime
    , B.CalcRule AS bimmodel_CalcRule
  FROM hgdataserver.PROJECT P
    , hgdataserver.BIMMODEL B
    , hgdataserver.TREENODE T
  WHERE P.RecordID = B.ProjectID
    AND P.ParentID = T.RecordID
    AND FIND_IN_SET(B.RecordID, record_ids)
  ;

END
;;
DELIMITER ;

SELECT '  创建 获取 OCCBOOL 算量用 工程情报信息(proc_get_occbool_projects_calc)' AS '成功:';

-- CALL proc_get_occbool_projects_calc('2362|2352|2383');