
set serveroutput on

declare owner_name varchar(32);
cursor cur1 is select table_name from user_tables;
begin
	for cur2 in cur1 loop
		execute immediate 'drop table '||cur2.table_name||' cascade constraints';
	end loop;
end;
/

declare owner_name varchar(32);
cursor cur1 is select sequence_name from user_sequences;
begin
	for cur2 in cur1 loop
		execute immediate 'drop sequence '||cur2.sequence_name;
	end loop;
end;
/

exit
