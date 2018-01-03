#!/bin/bash

cur_dir="$(dirname $0)"
cd $cur_dir

global_database_name="orcl"
if [ -f /etc/oratab ];then
	global_database_name=`cat /etc/oratab|tail -n 1|awk -F: '{print $1}'`
fi

echo "${global_database_name}" > dbname
echo "startup" > start-oracle.sql
echo "exit" >> start-oracle.sql

chmod 777 dbname
chmod 777 start-oracle.sql

su oracle -c "
		. /usr/local/bin/oraenv < dbname 1>/dev/null
		sqlplus \"/ as sysdba\" @start-oracle.sql
		"

. /usr/local/bin/oraenv < dbname 1>/dev/null
rm -rf dbname start-oracle.sql
cd - 1>/dev/null

