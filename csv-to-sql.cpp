/*
Timothy Queva
CS2910 Lab6
March 26, 2021

This program reads a csv file and converts it into a SQLite database file.
*/

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <stdexcept>

using namespace std;

void dumptblName(string data){
	cout << data << endl;
}

void dumpvstring(vector<string> data){
	for(auto i : data) cout << setw(12) << left << i;
	cout << endl;
}

int buildTblCommand(string &ssql,vector<string> &schema,string &tblName){
	string line="";
	bool first = true;
	
	//Gets the table name and starts building sql command
	//eg. ssql = "create table EMPLOYEE ("
	getline(cin,tblName);
	ssql.append(tblName + "(");
	
	//Gets the table schema and continues building sql command:
	//eg. FNAME:A15 ---converted-to---> FNAME char(15),
	getline(cin,line);
	boost::split(schema,line,boost::is_any_of(","));
	for(auto i : schema){
		try{
			//adds field name (eg. FNAME) to ssql string
			ssql.append(i.substr(0,i.find(":")));
		
			//changes datatype I,F,An (eg. A15) to int,real,char(n) and adds it to sql command with comma
			if(i.substr(i.find(":")+1) == "I" || i.substr(i.find(":")+1) == "i"){
				if(first){
					ssql.append(" int primary key not null,");
					first = false;
				}
				else{
					ssql.append(string(" ").append("int,"));
				}
			}
			else if(i.substr(i.find(":")+1) == "F" || i.substr(i.find(":")+1) == "f"){
				ssql.append(string(" ").append("real,"));
			}
			else if(i.substr(i.find(":")+1).find("A") != -1 || i.substr(i.find(":")).find("a") != -1){
				ssql.append(string(" ") + "char(" + i.substr(i.find(":")+2) +"),");
			}
			else return -1;
		}
		//catches error caused by missing semicolon
		catch(const out_of_range& e){
			return 0;
		}
	}
	ssql.erase(ssql.length()-1);	//deletes last comma
	ssql.append(");");				//adds closing parenthesis and semicolon
	
	return 1;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void buildInsertCommand(string &ssql,vector<string> schema,string tblName){
	vector<string> attributes;
	string line="";
	int count = 0;
	
	//Builds the first part of the insert command up to values keyword:
	//insert into <table>(<schema>) values
	ssql = "";
	ssql.append(string("insert into ") + tblName + "(");
	for(auto i : schema){
		ssql.append(i.substr(0,i.find(":")) + ",");
	}
	ssql.erase(ssql.length()-1);	//deletes last comma
	ssql.append(") values ");
	
	//Gets attributes and continues building sql insert command
	while(getline(cin,line)){
		//Stops gathering inputs if five pounds signs found
		if(line.find("#####") != -1) break;
		
		//string processing
		boost::split(attributes,line,boost::is_any_of(","));
		
		//dumps file attribute contents to screen
		dumpvstring(attributes);
		
		//Adds properly formatted tuples to ssql command string
		count = 0;
		ssql.append("(");
		for(auto i : attributes){
			if(schema[count].find(":A") != -1 || schema[count].find(":a") != -1){
				ssql.append("\'" + i + "\',");
			}
			else ssql.append(i + string(",")); 
			count++;
		}
		ssql.erase(ssql.length()-1);	//deletes last comma
		ssql.append("),");
	}
	ssql.erase(ssql.length()-1);
	ssql.append(";");
}


int main(){
	sqlite3 *db;
	char *zErrMsg = 0;
	vector<string> schema;
	string tblName  = "";
	string ssql="create table ";
	char *sql;
	
	//Prompts for user
	cout << "If you just see a blinking cursor, this means you forgot to give a file to this program." << endl;
	cout << "Press ctrl + d or ctrl + c to end program and then type: ./dotlite < test.db\n" << endl;
	
	//Builds the create table sql command from first two lines of file.
	int rc = buildTblCommand(ssql,schema,tblName);
	if(rc==0){
		cout << "Contents of file so far:" << endl;
		dumptblName(tblName);
		dumpvstring(schema);
		
		sqlite3_close(db);
		cout << endl << endl;
		cout << "Please recheck the your schema: missing colon." << endl;
		cout << "PROGRAM ABORTED" << endl;
		exit(-1);
	}
	else if(rc == -1){
		cout << "Contents of file so far:" << endl;
		dumptblName(tblName);
		dumpvstring(schema);
		
		sqlite3_close(db);
		cout << endl << endl;
		cout << "Sorry, please recheck your schema: missing or incorrect datatype." << endl;
		cout << "PROGRAM ABORTED" << endl;
		exit(-1);
	}
	
	//dumps first two lines from file to console
	cout << "Contents of file:" << endl;
	dumptblName(tblName);
	dumpvstring(schema);
	
	//Creates sql lite table
	sql = (char*) ssql.c_str();
	sqlite3_open("test.lite", &db);
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if(rc != SQLITE_OK){
		cout << endl;
		cerr << "SQL error: " << zErrMsg << endl << endl;
		sqlite3_free(zErrMsg);
		
		cout << "SQL command: " << ssql << endl << endl;
		cout << "PROGRAM ABORTED" << endl;
		sqlite3_close(db);
		exit(-1);
	}
	
	//Builds the insert-into-table command
	buildInsertCommand(ssql,schema,tblName);
	
	//Inserts tuples into sql table.
	sql = (char*) ssql.c_str();
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if(rc != SQLITE_OK){
		cout << endl;
		cerr << "SQL error: " << zErrMsg << endl << endl;
		sqlite3_free(zErrMsg);
		
		cout << "SQL command: " << ssql << endl << endl;
		cout << "PROGRAM ABORTED-attributes not added." << endl;
		sqlite3_close(db);
		exit(-1);
	}
	else{
		cout << "\nDATABASE IMPORT SUCCESSFUL" << endl;
		cout << "csv-formatted file successfully converted into SQLite database." << endl;
	}
	
	sqlite3_close(db);
	
	return 0;
}