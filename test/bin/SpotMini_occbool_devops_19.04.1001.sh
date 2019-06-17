#!/usr/bin/env bash

# To move the old log files
LOG_PATH=/opt/log/
cd ${LOG_PATH}
dateFolder=`date "+%Y-%m-%d"`
if [ ! -d "${dateFolder}" ];then
        mkdir ${dateFolder}
        mv *.log ${dateFolder}
        echo "Moved old log files into folder: ${dateFolder}"
else
        echo "Already has folder: ${dateFolder}"
fi

cd `dirname $0`

export LD_LIBRARY_PATH=/opt:$LD_LIBRARY_PATH

# 设定测试的数据库连接信息
export CI_CONNECTOR_MYSQL_CONNECTION=root:mysqladm@192.168.1.35:23306

# 请求用 算量 RabbitMQ 服务相关情报(OCCBOOL 算量用)
#   设定格式: user:password@host:port
export CI_MQ_CALC_REQUEST_HOST_CONN_PARAMS=guest:guest@rabbitmq:5672

# 请求用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
export CI_MQ_CALC_REQUEST_CMD_NAME=CALC_OCCBOOL_DEVOPS_1010

# 回复用 算量 RabbitMQ 服务相关情报(OCCBOOL 算量用)
#   设定格式: user:password@host:port
export CI_MQ_CALC_RESPONSE_HOST_CONN_PARAMS=guest:guest@rabbitmq:5672

# 回复用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
export CI_MQ_CALC_RESPONSE_CMD_NAME=CALC_OCCBOOL_DEVOPS_2010

# OCCBOOL 算量用 工程情报(OCCBOOL 算量用)
#   数据来源参照 hgdataserver.BIMMODEL.RecordID
#   设定格式: RecordID|RecordID|RecordIDrem 请求用 算量 RabbitMQ 队列名称(OCCBOOL 算量用)
## 1.4.6.0415测试文件
## 工程信息: 宛平路2 1
## 计算规则: 轨道交通BIM分类清单计算规则20190408
## 适用 OCCBOOL REALEASE-V1.4
export CI_OCCBOOL_PROJECT_BIMMODEL_RECORD_IDS=2643

# OCCBOOL 算量用 计算构件类别情报(OCCBOOL 算量用)
#   数据来源参照 hg::bim::Category
#   设定格式: Category|Category|Category
# 11: 混凝土楼梯
# 29: 桩
# 37: 混凝土回填
# 39: 混凝土腋角
# 53: 其他构件
# 55: 防水
export CI_OCCBOOL_PROJECT_BIMMODEL_CATEGORIES='1|3|5|7|11|29|37|39|53|55'

# 待计算 OCCBOOL 算量队列为空, 等待耗时构件算量的完成或异常的超时时间
#   单位: 分钟
#   设定范围(整型): [1, 60)
export CI_WAIT_TIMEOUT_FROM_EMPTY_QUEUE=10

# 算量结果对比用的 公差 情报
#   设定类型: 浮点型(设定参照 MySQL 关于 double(m, d) 定义)
export CI_TOLERANCE=0.005

# 算量用 同类型构件计算最大个数 情报
#   设定类型: 无符号型(0: 全计算)
export CI_MAX_COUNT_CATEGORY=0

./SpotMini_occbool_devops_19.04.1001
