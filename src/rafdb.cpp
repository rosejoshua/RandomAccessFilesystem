//-----------------------------------------------------
// Example code to read from fixed length records (random access file)
// Author:  Joshua Rose, modified version of code by Dr. Susan Gauch
// Last updated:  Jan. 31, 2022
//-----------------------------------------------------

#include "rafdb.h"

RafDb::RafDb()
{
  numSortedRecords = 0;
  numOverflow = 0;
  recordSize = 0;
}

RafDb::~RafDb() {}

// void RafDb::setFieldsAndMaxLengths(vector< pair<string,int> > fieldsAndMaxLengths)
// {
//   this->fieldsAndMaxLengths = vector< pair<string,int> >(fieldsAndMaxLengths);
// }

// void RafDb::setNumSortedRecords(int numSortedRecords)
// {
//   this->numSortedRecords = numSortedRecords;
// }

// void RafDb::setNumOverflowRecords(int numOverflowRecords)
// {
//   this->numOverflow = numOverflowRecords;
// }
void RafDb::getDefaultFields(vector<string> &fields)
{
  fields.clear();
  for (int i=0; i<fieldsAndMaxLengths.size(); i++)
  {
    fields.push_back(string(fieldsAndMaxLengths[i].first));
  }
}

void RafDb::updateRecordSize()
{
  for (int i=0; i<fieldsAndMaxLengths.size(); i++)
  {
      recordSize += fieldsAndMaxLengths[i].second;
  }
  recordSize +=1;
}

bool RafDb::open(const string &filename)
{
  ifstream dIn;
  string tempString1;
  string tempString2;

  dIn.open(filename+".config");
  if(!dIn)
  {
    cerr << "Error opening \'" << filename << ".config\'\n\tHint: does file exist?" << endl;
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

  //checking if file exists on ifstream before opening on read/write fstream following because fStream will
  //create the file if it doesn't exist and we don't want that.
  dIn.open(filename+".data");
  if(!dIn)
  {
    dIn.close();
    return false;
  }
  dIn.close();

  // Open file in read/write mode
  fStream.open(filename + ".data", fstream::in | fstream::out | fstream::app);
  if(fStream.is_open())
  {

    updateRecordSize();
    return true;
  }
  else return false;
}

bool RafDb::isOpen()
{
    return (fStream.is_open());
}

void RafDb::close()
{
  if (fStream.is_open())
    fStream.close();
}

bool RafDb::searchByToken(const string &target, vector<string> &fields)
{
  int index = binarySearch(target, fields);
  return (index != -1);
  return false;
}

bool RafDb::readRecord(const int recordNum, vector<string> &fields)
{
  bool status = false;

  if ((0 <= recordNum) && (recordNum < numSortedRecords + numOverflow))
  {
    fStream.seekg(recordNum * recordSize, ios::beg);
    for(int i=0; i<fields.size(); i++)
    {
      fStream.width(fieldsAndMaxLengths[i].second);
      fStream >> fields[i];
    }
      
    //fStream >> name >> rank >> city >> state >> zip >> employees;
    status = true;
  }
  else
    cout << "Record " << recordNum << " out of range.\n";

  return status;
}

bool RafDb::writeRecord(const string &name, const string &rank, const string &city, const string &state, const string &zip, const string &employees)
{
  if (fStream.is_open())
  {
    // fStream << setw(10) << left << name 
    //      << setw(5) << left << rank 
    //      << setw(5) << left << city 
    //      << setw(20) << left << state 
    //      << setw(30) << left << zip 
    //      << setw(30) << left << employees
    //      << endl;

    return true;
  }
  else return false;
}

int RafDb::binarySearch(const string &targetName, vector<string> &fields)
{
  int low = 0;
  int high = NUM_RECORDS - 1;
  int mid;
  bool failure = false;

  bool found = false;
  while (!found && (high >= low) && !failure)
  {
    mid = (low + high) / 2;
    //cout << "mid: " << mid << " , midId: " << midId << ":" << endl;
    if (readRecord(mid, fields))
    {
      if (fields[0].compare(targetName) == 0)
        found = true;
      else if (fields[0].compare(targetName) < 0)
        low = mid + 1;
      else
        high = mid - 1;
    }
    else
    {
      cout << "Could not get record " << mid << endl;
      failure = true;
    }
  }

  if (found)
    return mid; // the record number of the record
  else
    return -1;
}
