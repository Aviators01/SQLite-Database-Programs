Timothy Queva
CS2910 Lab6
March 26, 2021

Description: There are two included programs here. The first one which is the dotlite program (csv-to-sql.cpp),
takes a csv-formatted file and converts it into a SQLite database file. The second program which is the
searchlite program (searchlitedb.cpp), provides an interface via console for the user to query the database.

Limitations:
	1) Only csv formatted files (file extension: .db) can be used with the first program.
	2) .db file must conform to the format specified at page bottom. Incorrect number of
	   pounds signs to indicate end of file will cause SQL command to fail (0 or 5 pound
	   signs must be implemented).

Instructions: Navigate to the correct file folder.
	1. Compile and link by typing: make
	2. Run first program by typing: ./dotlite < test.db
	3. Run second program by typing: ./searchlite
	4. Exit second program by typing: quit <OR> exit

------------------------------------------------------------------------------------------------------------------
Additional tips:
-The "test.db" in instruction #2 can be substituted for any csv-formatted file.
-The dotlite program is hardcoded to save the converted .db file as test.lite
-The searchlite program is hardcoded to open the test.lite database file for queries.

.db format:
-First line is table name
-Second line is schema with the first attribute assumed to be the primary key
-End of file indicated with 5 pound signs (#####) on the last line. (Technically, no
pounds signs could be implemented and program will still run-however, a blank line or
line feed will cause sql command to fail).