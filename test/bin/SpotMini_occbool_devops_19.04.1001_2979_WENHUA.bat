@echo off

REM 设定测试的数据库连接信息 
SET CI_CONNECTOR_MYSQL_CONNECTION=root:mysqladm@192.168.1.111:3306

REM 请求用 算量 RabbitMQ 服务相关情报(OCCBOOL 算量用)
REM   设定格式: user:password@host:port
SET CI_MQ_CALC_REQUEST_HOST_CONN_PARAMS=guest:guest@192.168.1.113:5672

REM 请求用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
SET CI_MQ_CALC_REQUEST_CMD_NAME=CALC

REM 回复用 算量 RabbitMQ 服务相关情报(OCCBOOL 算量用)
REM   设定格式: user:password@host:port
SET CI_MQ_CALC_RESPONSE_HOST_CONN_PARAMS=guest:guest@192.168.1.113:5672

REM 回复用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
SET CI_MQ_CALC_RESPONSE_CMD_NAME=CALC_OCCBOOL_DEVOPS_2010_WENHUA

REM OCCBOOL 算量用 工程情报(OCCBOOL 算量用)
REM   数据来源参照 hgdataserver.BIMMODEL.RecordID
REM   设定格式: RecordID|RecordID|RecordIDrem 请求用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
SET CI_OCCBOOL_PROJECT_BIMMODEL_RECORD_IDS=2979

REM OCCBOOL 算量用 计算构件类别情报(OCCBOOL 算量用)
REM   数据来源参照 hg::bim::Category
REM   设定格式: Category|Category|Category
SET CI_OCCBOOL_PROJECT_BIMMODEL_CATEGORIES=1^|3^|5^|7^|11^|29^|37^|39^|53^|55

REM 待计算 OCCBOOL 算量队列为空, 等待耗时构件算量的完成或异常的超时时间
REM   单位: 分钟
REM   设定范围(整型): [1, 60)
SET CI_WAIT_TIMEOUT_FROM_EMPTY_QUEUE=30

REM 算量结果对比用的 公差 情报
REM   设定类型: 浮点型(设定参照 MySQL 关于 double(m, d) 定义)
SET CI_TOLERANCE=0.005

REM 算量用 同类型构件计算最大个数 情报
REM   设定类型: 无符号型(0: 全计算)
SET CI_MAX_COUNT_CATEGORY=0

____SpotMini_occbool_devops_19.04.1001.exe
