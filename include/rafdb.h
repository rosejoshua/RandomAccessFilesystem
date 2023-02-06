//-----------------------------------------------------
// Example code to read from fixed length records (random access file)
// Author:  Joshua Rose, modified version of code by Dr. Susan Gauch
// Last updated:  Jan. 31, 2022
//-----------------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class RafDb
{
private:
  int numSortedRecords;
  int numOverflow;
  int recordSize;
  fstream *p_dbFilePtr;
  vector< pair<string,int> > fieldsAndMaxLengths;

  void spaceToUnderscore(string &text);
  void underscoreToSpace(string &text);
  void updateEntryWidth();
  void printHeader();
  void printFooter();
  int getMinWidthField(const int index);
  bool readRecord(const int recordNum, vector<string> *fields);
  void printRecord(vector<string> *results);
  bool writeRecord(vector<string> *fields);
  int binarySearch(const string &targetName, vector<string> *fields);

public:
  const static bool ON_MS_WINDOWS_OS = false;

  RafDb();
  ~RafDb();

  bool createDB(const string inFilename);
  bool searchByToken(string &target, vector<string> *fields);
  void getDefaultFields(vector<string> *fields);
  bool updateRecord(vector<string> *fields);
  bool open(const string &filename);
  bool isOpen();
  void close();
  void runTests();
};