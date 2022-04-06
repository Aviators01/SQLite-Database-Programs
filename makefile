ALL: dotlite searchlite

dotlite:
	g++ -std=c++11 csv-to-sql.cpp -o dotlite -lboost_serialization -l sqlite3

searchlite:
	g++ -std=c++11 searchlitedb.cpp -o searchlite -lboost_serialization -l sqlite3

clean:
	rm dotlite searchlite test.lite