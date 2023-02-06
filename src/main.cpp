#include <iostream>
#include <rafdb.h>
#include <vector>

void menuOpt1(RafDb &db)
{
   string inputCsvName; 
   cout << "Please enter the input .csv name without file extension: ";
   getline(cin, inputCsvName, '\n');
   cin.clear();
   db.createDB(inputCsvName);
}

void menuOpt2(RafDb &db)
{
   string dbName; 
   cout << "Please enter the name of a database to open: ";
   getline(cin, dbName, '\n');
   cin.clear();

   if (db.open(dbName))
   {
      cout << "Succesfully opened " << dbName << ".data" << endl;
   }
   else 
   {
      cerr << "Failure opening \'" << dbName << ".data\'\n\tHint: Has database been created yet?" << endl;
   }
}

void menuOpt3(RafDb &db, vector<string> *fields)
{
   cout << "Closing any open database files." << endl;
   fields->clear();
   db.close();
}

void menuOpt4(RafDb &db, vector<string> *fields)
{
   string searchTarget="";
   cout << "Please enter the name to search (case-sensitive): ";
   getline(cin, searchTarget, '\n');
   cin.clear();
   
   db.getDefaultFields(fields);

   if (!db.searchByToken(searchTarget, fields))
      cout << "No results found." << endl;
}

int main(){
   RafDb db;
   db.runTests();

	cout << "exiting..." << endl;
	return 0;
}