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

void resolveMenuMethod(int choice)
{
   switch (choice)
   {
   case 0:
      printMenu();
      break;
   case 1:
      cout << "chose 1" << endl;
      //call method
      break;
   case 2:
      cout << "chose 2" << endl;
      //call method
      break;
   default:
      break;
   }
}

int main()
{
   bool running = true;
   RafDb db;
   string currentInput;
   vector<string> fields;
   int currentChoice;

   while (running)
   {
      currentChoice = 0;
      cout << ">> " ;
      getline(cin, currentInput);
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
         resolveMenuMethod(currentChoice);
      }
   }

   //db.runTests();
	cout << "exiting..." << endl;
	return 0;
}