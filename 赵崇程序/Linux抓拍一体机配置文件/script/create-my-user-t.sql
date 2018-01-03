create database VDB;

grant all privileges on VDB.* to "VUSER"@"localhost" identified by "VPWD";
grant all privileges on VDB.* to "VUSER"@"VHOSTNAME" identified by "VPWD";
grant all privileges on VDB.* to "VUSER"@"%" identified by "VPWD";

flush privileges;

