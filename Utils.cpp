#include <map>
#include <list>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>

using namespace std;

#define strtoval() sstr << str; sstr >> retVal;
#define value_type_t typename Container::value_type

//sorting
template <class Container>
void aqsort(Container & c, const size_t left, const size_t right)
{
  size_t i = left, j = right;

  if(j - i <= 1) return;
  else
  {
    value_type_t pivot = c.at((left + right)/2);

    while(i < j)
    {
      while(c.at(i) < pivot) i++;
      while(c.at(j) > pivot) j--;

      if(i < j)
      {
        value_type_t tmp = c.at(i);
        c.at(i++) = c.at(j);
        c.at(j--) = tmp;
      }

      if(left < j) aqsort(c, left, j);
      if(i < right) aqsort(c, i, right);
    }
  }
}

//data type coersion
template <typename T1, typename T2>
T2 cast(T1 from, T2 to)
{
   to = reinterpret_cast<T2>(from);
   return to;
}

template <typename T3>
T3 CastTo(const void* buff, T3 & val)
{
  T3 v;
  v = (*(T3*)buff);
  return v;
}

template <typename T4>
T4 StringTo(const string & s, T4 & val)
{
  stringstream sstr;
  sstr << s;
  sstr >> val;
  return val;
}

short CastToShort(const void* buff)
{
  short v;
  v = (*(short*)buff);
  return v;
}

unsigned short CastToUnsignedShort(const void* buff)
{
  unsigned short v;
  v = (*(unsigned short*)buff);
  return v;
}

long CastToLong(const void* buff)
{
  long v;
  v = (*(long*)buff);
  return v;
}

unsigned long CastToUnsignedLong(const void* buff)
{
  unsigned long v;
  v = (*(unsigned long*)buff);
  return v;
}

long long CastToLongLong(const void* buff)
{
  long long v;
  v = (*(long long*)buff);
  return v;
}

unsigned long long CastToUnsignedLongLong(const void* buff)
{
  unsigned long long v;
  v = (*(unsigned long long*)buff);
  return v;
}

float CastToFloat(const void* buff)
{
  float v;
  v = (*(float*)buff);
  return v;
}

double CastToDouble(const void* buff)
{
  double v;
  v = (*(double*)buff);
  return v;
}

short StringToShort(const string & str)
{
  stringstream sstr;
  short retVal;
  strtoval();
  return retVal;
}

int StringToInt(const string & str)
{
  stringstream sstr;
  int retVal;
  strtoval();
  return retVal;
}

long long StringToLong(const string & str)
{
  stringstream sstr;
  long long retVal;
  strtoval();
  return retVal;
}

float StringToFloat(const string & str)
{
  stringstream sstr;
  float retVal;
  strtoval();
  return retVal;
}

double StringToDouble(const string & str)
{
  stringstream sstr;
  double retVal;
  strtoval();
  return retVal;
}

//file io
string mkbinfilebuf(const string & fileName)
{
  string retVal = "";
  ifstream in(fileName.c_str(), ios::in | ios::binary);

  if(in.is_open())
  {
    in.seekg(0, ios::end);
    int len = in.tellg();
    if(len > 0)
    {
      in.seekg(ios::beg);
      unsigned int flen, uint;
      do
      {
        in.read((char*)&uint, sizeof(unsigned int));
        uint -= 999;
        retVal += (char) uint;
      }while(in.peek() != EOF);
    }
    in.close();
    in.clear();
  }

  return retVal;
}

string mkfilebuf(const string & fileName)
{
  string retVal = "";
  ifstream in(fileName.c_str());

  if(in.is_open())
  {
    in.seekg(0, ios::end);
    int len = in.tellg();
    if(len > 0)
    {
      char* buff = new char[len + 1];
      memset(buff, 0, len + 1);
      in.seekg(ios::beg);
      in.read(buff, len);
      retVal = buff;
      delete [] buff;
    }
    in.close();
    in.clear();
  }

  return retVal;
}

bool writebinfb(const string & fileName, const string & buffer)
{
  bool retVal = true;

  if(fileName == "" || buffer == "") retVal = false;
  else
  {
    ofstream out(fileName.c_str(), ios::out | ios::binary); //or out.open
    if(out.is_open())
    {
      unsigned int val;
      for(int i = 0; i < buffer.length(); i++)
      {
        val = (unsigned int) buffer[i] + 999;
        out.write((char*)&val, sizeof(unsigned int));
      }
      out.close();
      out.clear();
    }
    else
      retVal = false;
  }

  return retVal;
}

bool writefb(const string & fileName, const string & buffer)
{
  bool retVal = true;

  if(fileName == "" || buffer == "") retVal = false;
  else
  {
    ofstream out;
    out.open(fileName.c_str());
    if(out.is_open())
    {
      out << buffer;
      out.close();
      out.clear();
    }
    else retVal = false;
  }

  return retVal;
}

//string manip
char* cstrcp(const char* & src)
{
  char* retVal = 0;
  size_t len = src ? strlen(src) : 0;

  if(len > 0)
  {
    retVal = new char[len + 1];
    memset(retVal, 0, len + 1);
    memcpy(retVal, src, len);
  }

  return retVal;
}

//same as below but in place since src is mutable
string& strfpip(string & src, const string & fstr, const string & rstr)
{
  size_t pos = src.find(fstr);

  if(pos != string::npos) return src.replace(pos, fstr.length(), rstr);
  else
  {
    cerr << "ERROR: Search string " + fstr + " not found.\n";
    return src;
  }
}

string strfp(const string & src, const string & fstr, const string & rstr)
{
  string retVal = "";
  size_t pos = src.find(fstr);

  if(pos != string::npos) retVal = src.substr(0, pos) + rstr;
  else cerr << "ERROR: Search string " + fstr + " not found in " << src << '\n';

  return retVal;
}

string strsubstr(const string & src, const string & delim)
{
  string retVal = "";
  size_t pos = src.find_first_of(delim);

  if(pos != string::npos) retVal = src.substr(pos+1, src.length() - 1);
  else cerr << "ERROR: Delimiter " + delim + " not found in " << src << '\n';

  return retVal;
}

int main(int argc, char* argv[])
{
  string str = "The quick brown fox", 
         fstr = "fox", 
         rstr = "dog", 
         file = "utils.cpp", 
         buff = "";

  long l = 0, ll = 0;
  short s = 16;
  double d = 0.0, dd = 2, ddd = 0.0;
  buff = "9.99999";
  cout << "String Test...\n\nRetrieve substring after . in: " << file << '\n';
  string sub = strsubstr(file, ".");
  cout << "Substring of " << file << " using . delimiter: " << sub << endl;
  cout << "\nRetrieve substring after + in: " << file << '\n';
  string esub = strsubstr(file, "+");
  const char * original = "copytextstr";

  cout << "\nString Function Pointer\n";
  char* (*cscpfp) (const char* & src) = &cstrcp;
  cout << "function pointer address: " << (uint32_t)cscpfp
       << " function address: " << (uint32_t)cstrcp << '\n';
  char* copy = (*cscpfp) (original);
  if(copy)
  {
    cout << "After calling cstrcp() with: " << original << " copy value: "
         << copy << endl;
    delete [] copy;
    copy = 0;
    cout << "Allocated pointer destroyed\n\n";
  }
  cout << "String Find & Replace\n" << "Before:\t" << str
       << "\n After:\t" <<  strfp(str, fstr, rstr) << endl;
  string cpystr = str;
  cout << "\nString In Place Find & Replace\nBefore:\t" << cpystr << '\n';
  strfpip(cpystr, fstr, rstr);
  cout << " After:\t" << cpystr << '\n';
  cout << "\nString Find & Replace " << rstr << " with " << fstr << " in: "
       << str << '\n';
  strfp(str, rstr, fstr);

  cout << "\nCasting Test...\n\n";
  d = StringTo(buff, d);
  ddd = CastTo(&d, ddd);
  cout << "Casting double d " << d << " as void* to double yeilds: " << ddd << endl;
  cout << "Casting StringTo(" << buff << ") Double = " << d << endl;
  dd = StringToDouble(buff);
  cout << "Casting string " << buff << " to double yeilds: " << dd << endl;
  ll = (long)(*(cast((short*)&s, &ll)));
  cout << "Casting short* " << s << " to long* yeilds: " << ll << endl;
  dd = CastToDouble(&d); /*This is the same as
  dd = CastTo(&d, dd);*/
  cout << "Casting double " << d << " to double yeilds: " << dd << endl;

  cout << "\nSort Vector Test...\n\n";
  vector<int> ivector;
  for(int i = 7; i > -1; --i) ivector.push_back(i);
  cout << "unsorted ivector: ";
  for(int i = 0; i < ivector.size(); ++i) cout << ivector.at(i);
  aqsort(ivector, 0, ivector.size() - 1);
  cout << "\n  sorted ivector: ";
  for(int i = 0; i < ivector.size(); ++i) cout << ivector.at(i);

  cout << "\n\nFile IO Test...\n\nCreating file buffer\n";
  buff = mkfilebuf(file);
  file += ".bak";
  writefb(file, buff);
  cout << "Writing file buffer to: " << file << '\n';

  file = "./prop.txt";
  cout << "\nCreating duplicate binary file buffer\n";
  buff = mkfilebuf(file);
  file += ".bin";
  cout << "Writing duplicate binary file buffer to: " << file;
  writebinfb(file, buff);

  cout << "\n\nRecreating the file buffer from bin to txt";
  buff =  mkbinfilebuf(file);
  file += ".bak";
  cout << "\nWriting file buffer to: " << file << "\n\nfin\n\n";
  writefb(file, buff);

  return 0;
}
