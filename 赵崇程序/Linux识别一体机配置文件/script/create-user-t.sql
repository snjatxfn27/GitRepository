-- drop user VSQLUSER cascade;

create temporary tablespace VSPACE_temp
tempfile 'VORACLE_BASE/oradata/orcl/VSPACE_temp.dbf'
size 200m
autoextend on 
next 100m maxsize 1000m
extent management local;

create tablespace VSPACE_data
logging
datafile 'VORACLE_BASE/oradata/orcl/VSPACE_data.dbf'
size 4000m
autoextend on
next 1000m maxsize unlimited
extent management local;

create user VSQLUSER identified by VSQLPWD
default tablespace VSPACE_data
temporary tablespace VSPACE_temp;

grant connect,resource,dba to VSQLUSER;

-- alter tablespace VSPACE_data add datafile 'VORACLE_BASE/oradata/orcl/VSPACE_data2.dbf' size 32000m;

exit
