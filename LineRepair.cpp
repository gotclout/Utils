#include <iostream>
#include <string>
#include <fstream>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <dirent.h>
  #include <stdio.h>
#define MAX_PATH 512
#endif

using namespace std;

void repairDirectory(const char* baseDir, bool recurse);
bool repairFile(const char* filename);

/**
 * Main:
 *
 * TODO: Add CLI
 */
int main(int argc, char** argv)
{
  int retVal = 0;
  string repairType;
  ifstream infile;
  ofstream outfile;
  string filename, recurse;

  do
  {
    cout << "To repair a single file enter \"file\" or to repair all files in"
         << " a directory enter \"dir\"." << endl;
    cin >> repairType;
    if(repairType == "file")
    {
      cout << "Input the path to the file you would like to repair..." << endl;
      cin >> filename;

      while(!repairFile(filename.c_str()) && filename != "quit" && filename != "exit")
      {
        cout << "Invalid file name, input the file that you would like to repair..." << endl;
        cin >> filename;
      }
    }
    else if(repairType == "dir")
    {
      cout << "Input the directory that you would like to repair..." << endl;
      cin >> filename;
      cout << "Would you like to repair files in subdirectories? (yes or no)" << endl;
      cin >> recurse;

      while(recurse != "yes" && recurse != "no")
      {
        cout << "You have entered an invalid selection, enter yes to repair "
             << "files in subdirectories or no." << endl;
        cin >> recurse;
      }

      if(recurse == "yes")
      {
        repairDirectory(filename.c_str(), true);
      }
      else if(recurse == "no")
      {
        repairDirectory(filename.c_str(), false);
      }
    }
    else if(repairType != "quit" && repairType != "exit")
    {
      cout << "You have entered an invalid repair type" << endl;
    }
  }
  while(filename != "quit" && filename != "exit");

  return retVal;
}

/**
 *
 *
 */
bool repairFile(const char * filename)
{
  bool retVal = true;
  ifstream infile;
  ofstream outfile;
  string buffer, newfile = "";
  infile.open(filename);
  if(infile){
    getline(infile, buffer, '\r');
    newfile += buffer;
    while(!infile.eof()){
      getline(infile, buffer, '\r');
      newfile += buffer;
    }
    infile.close();
    outfile.open(filename);
    outfile << newfile;

    outfile.close();
  }
  else
  {
    retVal = false;
  }

  return retVal;
}

/**
 *
 *
 */
void repairDirectory(const char* baseDir, bool recurse)
{
  HANDLE            h;
  WIN32_FIND_DATA  info;
  char* currentDir = new char[MAX_PATH];
  string file;

  GetCurrentDirectory(MAX_PATH, currentDir);
  SetCurrentDirectory(baseDir);

  h = FindFirstFile("*.*", &info);
  if(h == NULL)
  {
    cout << "You have entered an invalid directory to repair." << endl;
  }
  else
  {
    while(h != INVALID_HANDLE_VALUE)
    {
      if(info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
         strcmp(info.cFileName, ".") && strcmp(info.cFileName, ".."))
      {
        if(recurse)
        {
          char newroot[MAX_PATH];
          sprintf(newroot, "%s\\%s", baseDir, info.cFileName);
          SetCurrentDirectory(info.cFileName);
          repairDirectory(newroot, recurse);
          SetCurrentDirectory("..");
        }
      }
      else
      {
        repairFile(info.cFileName);
      }

      if (!FindNextFile(h, &info))
      {
        //TODO: Fixme
        break;
      }
    }
  }

  SetCurrentDirectory(currentDir);

  delete [] currentDir;
}
