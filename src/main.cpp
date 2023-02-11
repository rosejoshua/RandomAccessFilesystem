#include <iostream>
#include <rafdb.h>
#include <vector>

using namespace std;

void menuOpt1(RafDb *db)
{
   string inputCsvName;
   db->close();
   cout << "Please enter the input .csv name without file extension: ";
   getline(cin, inputCsvName, '\n');
   cin.clear();
   cout << endl;
   cout << "CREATE DATABASE \"" << inputCsvName << "\"" << endl;
   db->createDB(inputCsvName);
}

void menuOpt2(RafDb *db)
{
   string dbName; 
   cout << "Please enter the name of a database to open: ";
   getline(cin, dbName, '\n');
   cin.clear();
   cout << endl;
   cout << "OPEN DATABASE \"" << dbName << "\"" << endl;

   if (db->open(dbName))
   {
      cout << "Succesfully opened " << dbName << ".data" << endl;
   }
   else 
   {
      cerr << "Failure opening \'" << dbName << ".data\'\n\tHint: Has database been created yet?" << endl;
   }
}

void menuOpt3(RafDb *db, vector<string> *fields)
{
   cout << "CLOSE OPEN DATABASE ANY" << endl;
   cout << "Closing any open database files." << endl;
   fields->clear();
   db->close();
}

void menuOpt4(RafDb *db, vector<string> *fields)
{
   string searchTarget="";
   cout << "Please enter the name to search (case-sensitive): ";
   getline(cin, searchTarget, '\n');
   cin.clear();
   cout << endl;
   cout << "DISPLAY COMPLETE RECORD \"" << searchTarget << "\"" << endl;

   db->getDefaultFields(fields);

   if (!db->searchByToken(searchTarget, fields) || fields->at(1).compare("-1") == 0)
      cout << "No results found." << endl;
   else
   {
      db->printHeader();
      db->printRecord(fields);
      db->printFooter();
   }
}

void menuOpt5(RafDb *db, vector<string> *fields)
{
   string searchTarget="";
   int index = 0;
   cout << "Please enter the name of record to edit (case-sensitive): ";
   getline(cin, searchTarget, '\n');
   cin.clear();
   cout << endl;

   db->getDefaultFields(fields);

   if (!db->searchByToken(searchTarget, fields) || fields->at(1).compare("-1") == 0)
   {
      cout << "UPDATE \"" << searchTarget << "\" RECORD" << endl;
      cout << "No results found." << endl;
   }
   else
   {
      db->printIndexHeader();
      db->printRecord(fields);
      db->printFooter();

      cout << "Index of element to edit (cannot edit index 0, key): ";
      getline(cin, searchTarget);
      cin.clear();
      cout << endl;
      try 
      {
         index = stoi(searchTarget);
         searchTarget = "";
      } 
      catch (const std::exception& e) 
      {
         index = 0;
         cout << "UPDATE \"" << searchTarget << "\" RECORD" << endl;
         cerr << "Invalid index input!" << endl;
         return;
      }
      if (index == 0)
      {
         cout << "UPDATE \"" << searchTarget << "\" RECORD" << endl;
         cerr << "Error: cannot edit index 0, key!" << endl;
         return;
      }
      else if (index < 0 || index > fields->size()-1)
      {
         index = 0;
         cout << "UPDATE \"" << searchTarget << "\" RECORD" << endl;
         cerr << "Error: input index out of range!" << endl;
         return;
      }

      cout << "Enter new value for element: ";
      getline(cin, searchTarget);
      cin.clear();
      cout << endl;

      if (searchTarget == "")
      {
         cout << "UPDATE \"" << fields->at(0) << "\" RECORD " << db->getColumnName(index) << " FROM " << fields->at(index) << " TO " << "\"\"" << endl;
         cerr << "Invalid empty input, try a space character if you want an empty field" << endl;
         return;
      }
      cout << "UPDATE \"" << fields->at(0) << "\" RECORD " << db->getColumnName(index) << " FROM \"" << fields->at(index) << "\" TO \"" << searchTarget << "\"" << endl;
      fields->at(index) = searchTarget;
      if (!db->updateRecord(fields))
      {
         cerr << "Failure editing record element!" << endl;
      }
   }
}

void menuOpt6(RafDb *db)
{
   int numRecords = 10;
   cout << endl;
   cout << "CREATE REPORT FIRST " << numRecords << " RECORDS" << endl;
   db->printFirstNumRecords(10);
}

void menuOpt8(RafDb *db)
{
   string deleteTarget="";
   cout << "Please enter the name of record to delete (case-sensitive): ";
   getline(cin, deleteTarget, '\n');
   cin.clear();
   cout << endl;

   if (db->deleteRecord(deleteTarget))
      cout << "Success deleting record: " << deleteTarget << endl;
   else
      cout << "Failure deleting record: " << deleteTarget << endl;
}

void printMenu()
{
   cout << "************* MENU **************" << endl;
   cout << "*                               *" << endl;
   cout << "*    #1) Create new database    *" << endl;
   cout << "*    #2) Open database          *" << endl;
   cout << "*    #3) Close database         *" << endl;
   cout << "*    #4) Display record         *" << endl;
   cout << "*    #5) Update record          *" << endl;
   cout << "*    #6) Create report          *" << endl;
   cout << "*    #7) Add record             *" << endl;
   cout << "*    #8) Delete record          *" << endl;
   cout << "*    #9) Quit                   *" << endl;
   cout << "*                               *" << endl;
   cout << "*********************************" << endl;
}

void resolveMenuMethod(int choice, RafDb* p_db, vector<string>* p_fields)
{
   cout << endl;
   switch (choice)
   {
   case 0:
      printMenu();
      break;
   case 1:
      //call method
      menuOpt1(p_db);
      break;
   case 2:
      //call method
      menuOpt2(p_db);
      break;
   case 3:
      //call method
      menuOpt3(p_db, p_fields);
      break;
   case 4:
      //call method
      if (p_db->isOpen())
      {
      menuOpt4(p_db, p_fields);
      }
      else 
         cerr << "No database is open yet!" << endl;
      break;
   case 5:
      //call method
      if (p_db->isOpen())
      {
      menuOpt5(p_db, p_fields);
      }
      else 
         cerr << "No database is open yet!" << endl;
      break;
   case 6:
      //call method
      if (p_db->isOpen())
      {
      menuOpt6(p_db);
      }
      else 
         cerr << "No database is open yet!" << endl;
      break;
   case 7:
      cout << "chose 7" << endl;
      break;
   case 8:
      //call method
      if (p_db->isOpen())
      {
      menuOpt8(p_db);
      }
      else 
         cerr << "No database is open yet!" << endl;
      break;
   default:
      break;
   }
}

int main()
{
   bool running = true;
   RafDb db;
   RafDb* p_db = &db;
   string currentInput;
   vector<string> fields;
   vector<string>* p_fields = &fields;
   int currentChoice;

   //db.runTests();

   while (running)
   {
      currentChoice = 0;
      cout << ">> " ;
      getline(cin, currentInput);
      cin.clear();
      try 
      {
         currentChoice = stoi(currentInput);
      } 
      catch (const std::exception& e) 
      {
         currentChoice = 0;
         //cout << "Invalid input, please try again..." << endl;
      }
      if (currentChoice < 0 || currentChoice > 9)
         cout << "Invalid input, please try again..." << endl;
      else if (currentChoice == 9)
         running = false;
      else
      {
         resolveMenuMethod(currentChoice, p_db, p_fields);
      }
   }

   //db.runTests();
	cout << "exiting..." << endl;
	return 0;
}