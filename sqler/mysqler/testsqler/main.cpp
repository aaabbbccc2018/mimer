#include<iostream>  
#include "../mysqler.h"  

int main()  
{  
	mysqler db("127.0.0.1", "test", "test", "testdb");
	db.connect();
	db.execsql("show create table testtb");
	db.show_result();
	db.free_result();
	db.execsql("insert testtb (Id,Name) values('1','test1')");
	db.show_result();
	db.free_result();
	db.execsql("insert testtb (Id,Name) values('2','test2')");
	db.show_result();
	db.free_result();
	db.execsql("select * from testtb");
	db.show_result();
	db.free_result();
	return 0;  
} 
