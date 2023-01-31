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
  fstream fStream;
  int numSortedRecords;
  int numOverflow;
  int recordSize;
  vector< pair<string,int> > fieldsAndMaxLengths;

  void updateRecordSize();
  bool readRecord(const int recordNum, vector<string> &fields);
  bool writeRecord(const string &name, const string &rank, const string &city, const string &state, const string &zip, const string &employees);
  int binarySearch(const string &targetName, vector<string> &fields);

public:
  const static bool ON_MS_WINDOWS_OS = false;

  const static int NAME_SIZE = 38;
  const static int RANK_SIZE = 3;
  const static int CITY_SIZE = 19;
  const static int STATE_SIZE = 2;
  const static int ZIP_SIZE = 5;
  const static int EMPLOYEES_SIZE = 7;

  const static int RECORD_SIZE = NAME_SIZE + RANK_SIZE + CITY_SIZE + STATE_SIZE + ZIP_SIZE + EMPLOYEES_SIZE + (ON_MS_WINDOWS_OS? 2:1);
  const static int NUM_RECORDS = 500;

  RafDb();
  ~RafDb();

  //void setFieldsAndMaxLengths(vector< pair<string,int> > fieldsAndMaxLengths);
  bool searchByToken(const string &target, vector<string> &fields);
  void getDefaultFields(vector<string> &fields);
  void setNumSortedRecords(int numSortedRecords);
  void setNumOverflowRecords(int numOverflowRecords);
  bool open(const string &filename);
  bool isOpen();
  void close();
};