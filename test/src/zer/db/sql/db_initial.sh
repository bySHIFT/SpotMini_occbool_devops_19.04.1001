#!/bin/bash
# Copyright (C) bySHIFT. All rights reserved.
# Licensed under the MIT license.
# =============================================================================
# db_initial.sh
# db initial script for ubuntu OS
# =============================================================================

cd `dirname $0`

PRODUCT="zer"
COMMAND_OPTIONS=""

INSTALL_DB_ZER_VER="19.04"

DB_VENDOR="MySQL"
DB_HOSTNAME="127.0.0.1"
DB_PORT="3306"
DB_USERNAME="root"
DB_PASSWORD="mysqladm"
MYSQL_LOGIN="mysql -h${DB_HOSTNAME} -P${DB_PORT} -u${DB_USERNAME} -p${DB_PASSWORD}"

function usage {
    echo "Usage: $0 [--install] [-h]"
    echo ""
    echo " --install <version>  DB version to install"
    echo "                        (default is ${INSTALL_DB_ZER_VER})"
    echo " -h,--help,-?         Display this information"
}

while [[ $# > 0 ]]
do
  case $1 in
  "--install")
    shift
    COMMAND_OPTIONS=$1
    ;;
  "-?"|"-h"|"--help")
    usage
    exit
    ;;
  *)
    usage
    exit 1
    ;;
  esac
  shift
done

# Variables setup
if [ "${COMMAND_OPTIONS}" == "" ]; then(
    usage
    exit 1
) fi

# -----
# DB install
# -----
if [ "${COMMAND_OPTIONS}" == "19.04" ]; then (
  echo ">> 检查 ${DB_VENDOR} 是否安装"
  mysql --version > /dev/null 2>&1
  if [ $? -eq 127 ];then
    echo "没有安装 ${DB_VENDOR}!"
  else
    echo "已安装${DB_VENDOR}!"
    mysql --version

    echo -e "\n"
    echo "开始初始化安装 ${PRODUCT} 数据库, 版本: ${COMMAND_OPTIONS}..."
    echo "请在以下输入 ${DB_VENDOR} 用户 ${USERNAME} 的密码"
    ${MYSQL_LOGIN}  < ./all.sql
  fi

) fi
