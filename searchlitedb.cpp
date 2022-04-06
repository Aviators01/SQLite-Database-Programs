/*
Timothy Queva
CS2910 Lab6
March 26, 2021

This program provides an interface for a user to
query/manipulate a SQLite database file.
*/

#include <sqlite3.h>
#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <cstring>

using namespace std;

//global variable
bool header = true;

static int callback(void *data, int argc, char **argv, char **azColName){
	//prints column header (ie. schema)
	if(header){
	   for(int i = 0; i<argc; i++) cout << setw(15) << left << azColName[i];
	   header=false;
	   cout << endl;
	}
	
	//prints attributes
	for(int i = 0; i<argc; i++){
		cout << setw(15) << left << (argv[i] ? argv[i] : "NULL");
	}
	cout << endl;
	
	return 0;
}

int main(){
	string keyboard="";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[256];
	
	rc = sqlite3_open("test.lite", &db);
	
	if(rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return(0);
	}
	
	cout << "When ready, begin typing your sql queries below." << endl;
	cout << "Hit ENTER to send request:" << endl << endl;
	
	while(1){
		//Captures input from keyboard
		cout << "-->: ";
		getline(cin,keyboard);
		strcpy(sql,keyboard.c_str());
		
		//provides a way for user to end program
		for(auto &i : keyboard){
			if(i>=65 && i<=90) i=i+32;
		}
		if(keyboard.find("exit") != -1 || keyboard.find("quit") != -1) break;
		
		//sends request to sql library
		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		if(rc != SQLITE_OK){
			cout << "Sorry, your query was not understood (";
			cerr << "SQL error: " << zErrMsg << ").";
			sqlite3_free(zErrMsg);
		}
		cout << endl;	//formatting
		
		//reset for next round
		header = true;
	}
	
	sqlite3_close(db);
	cout << "Goodbye!" << endl;
	
	return 0;
}