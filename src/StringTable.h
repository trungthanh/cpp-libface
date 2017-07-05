/*
 * Author: tngo@walmartlabs.com
 */

#ifndef __STRING_TABLE_H__
#define __STRING_TABLE_H__

#include <string>
#include <vector>

using namespace std;

class StringTable {
  public:
    vector<char*> table;   

    StringTable(int initSize=0) {
      if (initSize) {
        table.reserve(initSize);
      }
    }

    ~StringTable() {
      for (unsigned int i=0; i<table.size(); ++i) {
        delete[] table[i];
      }
    }

    int add(string &s) {
      char* newString = new char[(s.size()+1)];
      strcpy(newString, s.c_str());
      table.push_back(newString);
      return table.size() - 1;
    }

    char* get(unsigned int i) {
      return table[i];
    }

    void finalize() {
      vector<char*>(table).swap(table);
    }
};

#endif
