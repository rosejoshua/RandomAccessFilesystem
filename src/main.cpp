#include <iostream>
#include <rafdb.h>
#include <vector>

bool createDB(const string inFilename, RafDb *db)
{
   ifstream dIn;   // the csv file
   ofstream dOut;  // the data file
   //ofstream configOut;  // the config file
   vector< pair<string,int> > fieldsAndMaxLengths;
   string tempString1;
   string tempString2;
   int numSortedRecords = 0;
   int numOverflow = 0;

   dIn.open(inFilename+".config");
   if(!dIn)
   {
      cerr << "Error opening \'" << inFilename << ".config\'\n\tHint: does file exist?" << endl;
      return false;
   }

   getline (dIn, tempString1, ',');  // try to read first field of config file
   getline (dIn, tempString2);
   numSortedRecords = stoi(tempString1);
   numOverflow = stoi(tempString2);

   getline (dIn, tempString1, ',');
   while(!dIn.eof())
   {
      getline (dIn, tempString2);  // try to read a name
      fieldsAndMaxLengths.push_back(pair<string,int>(tempString1,stoi(tempString2)));

      getline (dIn, tempString1, ',');
   }
   dIn.close();

   dIn.open (inFilename+".csv");
   if(!dIn)
   {
      cerr << "Error opening \'" << inFilename << ".csv\'\n\tHint: does file exist?" << endl;
      return false;
   }
   dOut.open (inFilename+".data");

   getline(dIn, tempString1, ',');
   while(!dIn.eof())
   {
      for(int i=0; i<fieldsAndMaxLengths.size()-1; i++)
      {
         if (i!=0)
            getline(dIn, tempString1, ',');
         dOut << setw(fieldsAndMaxLengths[i].second) << left << tempString1;
      }
      getline(dIn, tempString1);
      dOut << setw(fieldsAndMaxLengths.back().second) << left << tempString1;
      dOut << endl;
      getline(dIn, tempString1, ',');
   }

   dIn.close();
   dOut.close();
   cout << "Successfully created " << inFilename << ".data from " << inFilename << ".csv" << endl;
   return true;
}

void menuOpt1(RafDb *db)
{
   string inputCsvName; 
   cout << "Please enter the input .csv name without file extension: ";
   cin >> inputCsvName;
   createDB(inputCsvName, db);
}

void menuOpt2(RafDb *db, vector<string> &fields)
{
   if (db->isOpen())
   {
      cerr << "Error: database already open. Please close before opening a new database." << endl;
      return;
   }

   string dbName; 
   cout << "Please enter the name of a database to open: ";
   cin >> dbName;

   if (db->open(dbName))
   {
      cout << "Succesfully opened " << dbName << ".data" << endl;
      db->getDefaultFields(fields);
   }
   else 
   {
      cerr << "Failure opening \'" << dbName << ".data\'\n\tHint: Has database been created yet?" << endl;
   }
}

void menuOpt3(RafDb *db, vector<string> &fields)
{
   cout << "Closing any open database files." << endl;
   fields.clear();
   db->close();
}

void menuOpt4(RafDb *db, vector<string> &fields)
{
   string searchTarget;
   db->getDefaultFields(fields);
   db->searchByToken("WHIRLPOOL", fields);
}

int main(){
   RafDb db;
   vector<string> fields;
//temp tests
   menuOpt1(&db);
   menuOpt2(&db, fields);
   menuOpt4(&db, fields);
   for(auto i : fields)
      cout << "" << i << endl;
   menuOpt3(&db, fields);

	cout << "exiting..." << endl;
	return 0;
}