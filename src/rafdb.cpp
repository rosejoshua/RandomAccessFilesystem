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
  p_dbFilePtr = new fstream();
}

RafDb::~RafDb()
{
  if (p_dbFilePtr != nullptr)
  {
    if (p_dbFilePtr->is_open())
      p_dbFilePtr->close();
    delete p_dbFilePtr;
  }
}

void RafDb::getDefaultFields(vector<string> *fields)
{
  fields->clear();
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    fields->push_back(string(fieldsAndMaxLengths[i].first));
  }
}

void RafDb::updateEntryWidth()
{
  recordSize = 0;
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    recordSize += fieldsAndMaxLengths[i].second;
  }
  recordSize += ON_MS_WINDOWS_OS ? 2 : 1;
}

bool RafDb::open(const string &filename)
{
  ifstream dIn;
  string tempString1;
  string tempString2;

  if (isOpen())
  {
    cerr << "Error: database already open. Please close before opening a new database." << endl;
    return false;
  }

  dIn.open(filename + ".config");
  if (!dIn)
  {
    cerr << "Error opening \'" << filename << ".config\'\n\tHint: does file exist?" << endl;
    return false;
  }

  getline(dIn, tempString1, ','); // try to read first field of config file
  getline(dIn, tempString2);
  numSortedRecords = stoi(tempString1);
  numOverflow = stoi(tempString2);

  getline(dIn, tempString1, ',');
  fieldsAndMaxLengths.clear();
  while (!dIn.eof())
  {
    getline(dIn, tempString2); // try to read a name
    fieldsAndMaxLengths.push_back(pair<string, int>(tempString1, stoi(tempString2)));
    getline(dIn, tempString1, ',');
  }
  dIn.close();

  // checking if file exists on ifstream before opening on read/write fstream following because fStream will
  // create the file if it doesn't exist.
  dIn.open(filename + ".data");
  if (!dIn)
  {
    dIn.close();
    return false;
  }
  dIn.close();

  // Open file in read/write mode
  p_dbFilePtr->open(filename + ".data", std::ios_base::in | std::ios_base::out | std::ios_base::ate);
  //p_dbFilePtr->open(filename + ".data", fstream::in | fstream::out | fstream::app);
  if (p_dbFilePtr->is_open())
  {
    updateEntryWidth();
    return true;
  }
  else
    return false;
}

bool RafDb::isOpen()
{
  if (p_dbFilePtr == nullptr || (!p_dbFilePtr->is_open()))
    return false;
  else
    return true;
}

void RafDb::close()
{
  if (p_dbFilePtr != nullptr && p_dbFilePtr->is_open())
    p_dbFilePtr->close();
}

void RafDb::spaceToUnderscore(string &text)
{
  std::replace(text.begin(), text.end(), ' ', '_');
}

void RafDb::underscoreToSpace(string &text)
{
  std::replace(text.begin(), text.end(), '_', ' ');
}

void RafDb::printRecord(vector<string> *results)
{
  string tempString = "";
  cout << "|";
  for (int i = 0; i < results->size(); i++)
  {
    tempString = results->at(i);
    underscoreToSpace(tempString);
    cout << setw(getMinWidthField(i)) << left << tempString;
    cout << "|";
  }
  cout << endl;
}


void RafDb::printRecordRange(const int low, const int high)
{
  vector<string> fields;
  getDefaultFields(&fields);
  if (high >= low)
  {
    printHeader();
    for (int i = low; i < high+1; i++)
    {
      readRecord(i, &fields);
      printRecord(&fields);
    }
    printFooter();
  }

  
}

int RafDb::getMinWidthField(const int index)
{
  if (!isOpen())
  {
    cerr << "No database open!" << endl;
    return 0;
  }
  int returnInt = fieldsAndMaxLengths[index].first.length() > fieldsAndMaxLengths[index].second ? fieldsAndMaxLengths[index].first.length() : fieldsAndMaxLengths[index].second;

  return returnInt;
}

void RafDb::printHeader()
{
  if (!isOpen())
  {
    cerr << "No database open!" << endl;
    return;
  }

  cout << "-";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    for (int j = 0; j < getMinWidthField(i); j++)
    {
      cout << "-";
    }
    cout << "-";
  }
  cout << endl;

  cout << "|";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    cout << setw(getMinWidthField(i)) << left << fieldsAndMaxLengths[i].first;
    cout << "|";
  }
  cout << endl;

  cout << "-";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    for (int j = 0; j < getMinWidthField(i); j++)
    {
      cout << "-";
    }
    cout << "-";
  }
  cout << endl;
}

void RafDb::printIndexHeader()
{
  if (!isOpen())
  {
    cerr << "No database open!" << endl;
    return;
  }

  cout << "-";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    for (int j = 0; j < getMinWidthField(i); j++)
    {
      cout << "-";
    }
    cout << "-";
  }
  cout << endl;

  cout << "|";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    cout << setw(getMinWidthField(i)) << left << i;
    cout << "|";
  }
  cout << endl;

  cout << "-";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    for (int j = 0; j < getMinWidthField(i); j++)
    {
      cout << "-";
    }
    cout << "-";
  }
  cout << endl;
}

void RafDb::printFooter()
{
  cout << "-";
  for (int i = 0; i < fieldsAndMaxLengths.size(); i++)
  {
    for (int j = 0; j < getMinWidthField(i); j++)
    {
      cout << "-";
    }
    cout << "-";
  }
  cout << endl;
}

bool RafDb::createDB(const string inFilename)
{
  ifstream dIn;  // the csv file
  ofstream dOut; // the data file
  string tempString1;
  string tempString2;
  int numInputSortedRecords = 0;
  int numInputOverflow = 0;

  dIn.open(inFilename + ".config");
  if (!dIn)
  {
    cerr << "Error opening \'" << inFilename << ".config\'\n\tHint: does file exist?" << endl;
    return false;
  }

  getline(dIn, tempString1, ','); // try to read first field of config file
  getline(dIn, tempString2);
  numInputSortedRecords = stoi(tempString1);
  numInputOverflow = stoi(tempString2);

  fieldsAndMaxLengths.clear();

  getline(dIn, tempString1, ',');
  while (!dIn.eof())
  {
    getline(dIn, tempString2); // try to read a name
    fieldsAndMaxLengths.push_back(pair<string, int>(tempString1, stoi(tempString2)));

    getline(dIn, tempString1, ',');
  }
  dIn.close();
  updateEntryWidth();

  dIn.open(inFilename + ".csv");
  if (!dIn)
  {
    cerr << "Error opening \'" << inFilename << ".csv\'\n\tHint: does file exist?" << endl;
    return false;
  }

  //empty out file if it already exists.
  dOut.open(inFilename + ".data");
  // finished with temporary ofStream object
  dOut.close();

  if (p_dbFilePtr != nullptr && p_dbFilePtr->is_open())
    p_dbFilePtr->close();

  p_dbFilePtr->open(inFilename + ".data", fstream::in | fstream::out | fstream::app);
  if (!p_dbFilePtr->is_open())
    return false;

  vector<string> entry;

  getline(dIn, tempString1, ',');
  while (!dIn.eof() && (numOverflow + numSortedRecords) < (numInputOverflow + numInputSortedRecords))
  {
    for (int i = 0; i < fieldsAndMaxLengths.size() - 1; i++)
    {
      if (i != 0)
        getline(dIn, tempString1, ',');
      spaceToUnderscore(tempString1);
      entry.push_back(tempString1);
    }
    getline(dIn, tempString1);
    spaceToUnderscore(tempString1);
    entry.push_back(tempString1);
    writeRecord(&entry);
    if (numSortedRecords < numInputSortedRecords)
      numSortedRecords++;
    else if (numOverflow < numInputOverflow)
      numOverflow++;
    entry.clear();
    getline(dIn, tempString1, ',');
  }
  cout << "Successfully created " << inFilename << ".data from " << inFilename << ".csv" << endl;
  dIn.close();
  
  close();
  return true;
}

bool RafDb::searchByToken(string &target, vector<string> *fields)
{
  spaceToUnderscore(target);
  int index = binarySearch(target, fields);
  return (index != -1);
}

bool RafDb::updateRecord(vector<string> *fields)
{
  if (isOpen())
  {
    vector<string> tempFields;
    tempFields = *fields;
    int index = -1;
    if (findRecord(&index, &tempFields))
    {
      if (index != -1)
      {
        p_dbFilePtr->seekp(index * recordSize);
        bool success = writeRecord(fields);
        if (success)
        {
          string target = tempFields[0];
          underscoreToSpace(target);
          cout << "Success updating entry with key: " << target << endl;
        }
        return success; //record found and updated
      }
    }
    return false; //record not found
  }
  return false; //database not open
}

bool RafDb::deleteRecord(const string &name)
{
  bool deleted = false;
  int index = -1;
  vector<string> tempFields;
  getDefaultFields(&tempFields);
  tempFields[0] = name;
  findRecord(&index, &tempFields);
  if (index != -1)
  {
    for(int i = 1; i<tempFields.size(); i++)
    {
      tempFields[i] = "-1";
    }
    cout << "success deleting" << endl; //delete this
    printRecord(&tempFields); // delete this
    p_dbFilePtr->seekp(index * recordSize);
    deleted = writeRecord(&tempFields);
  }
  return deleted;
}

bool RafDb::findRecord(int *recordNum, vector<string> *fields)
{
  if (isOpen())
  {
    string target = fields->at(0);
    spaceToUnderscore(target);
    *recordNum = binarySearch(target, fields);
    if (*recordNum == -1)
    {
      getDefaultFields(fields);
    }
    return true;
  }
  cerr << "Error: trying to find record without an opened database!" << endl;
  return false;
}

bool RafDb::readRecord(const int recordNum, vector<string> *fields)
{
  if (!isOpen())
  {
    cerr << "Error: trying to read record without an opened database!" << endl;
    return false;
  }
  bool status = false;
  string str = "";
  if ((0 <= recordNum) && (recordNum < numSortedRecords + numOverflow))
  {
    p_dbFilePtr->seekg(recordNum * recordSize, ios::beg);
    for (int i = 0; i < fields->size(); i++)
    {
      p_dbFilePtr->width(fieldsAndMaxLengths[i].second);
      *p_dbFilePtr >> str;
      fields->at(i) = str;
      str = "";
    }
    status = true;
  }
  else
    cout << "Record " << recordNum << " out of range.\n";

  return status;
}

bool RafDb::writeRecord(vector<string> *fields)
{
  
  if (!isOpen())
  {
    cerr << "Error: Attempting file write without an opened file!" << endl;
    return false;
  }
  else
  {
    string tempString;
    for (int i = 0; i < fields->size(); i++)
    {
      tempString = fields->at(i).substr(0,fieldsAndMaxLengths[i].second);
      spaceToUnderscore(tempString);
      *p_dbFilePtr << setw(fieldsAndMaxLengths[i].second) << left << tempString;
    }
    *p_dbFilePtr << endl;
    return true;
  }
}

int RafDb::binarySearch(const string &targetName, vector<string> *fields)
{
  int low = 0;
  int high = numSortedRecords - 1;
  int mid;
  bool failure = false;
  bool found = false;
  while (!found && (high >= low) && !failure)
  {
    mid = (low + high) / 2;
    if (readRecord(mid, fields))
    {
      if (fields->at(0).compare(targetName) == 0)
        found = true;
      else if ((fields->at(0)).compare(targetName) < 0)
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

void RafDb::runTests()
{
  createDB("Fortune500");
  open("Fortune500");
  vector<string> fields;
  getDefaultFields(&fields);
  if (readRecord(0, &fields))
  {
    printHeader();
    printRecord(&fields);
  }

  if (readRecord(9, &fields))
  {
    printRecord(&fields);
  }
  if (readRecord(5, &fields))
  {
    printRecord(&fields);
  }
  printFooter();
  if (readRecord(-1, &fields))
  {
    printHeader();
    printRecord(&fields);
    printFooter();
  }
  if (readRecord(1000, &fields))
  {
    printHeader();
    printRecord(&fields);
    printFooter();
  }

  if (readRecord(9, &fields))
  {
    printRecord(&fields);
  }
  fields[1] = "999";
  updateRecord(&fields);
  if (readRecord(9, &fields))
  {
    printRecord(&fields);
  }

  if (readRecord(499, &fields))
  {
    printRecord(&fields);
  }
  fields[2] = "fjkdlajkdjfs jkldsjadkf  djklajfklsjkljfdklsa jkfdlsjaflkdsj";
  updateRecord(&fields);
  if (readRecord(499, &fields))
  {
    printRecord(&fields);
  }
  string s = "WESTERN_REFINING";
  searchByToken(s, &fields);

  s = "WALMART";
  searchByToken(s, &fields);
  printRecord(&fields);

  deleteRecord(s);
  searchByToken(s, &fields);

  close();
}