/*
 * Author: tngo@walmartlabs.com
 */

#ifndef __QUERY_TABLE_H__
#define __QUERY_TABLE_H__

#include <string>
#include <vector>
#include "QueryInfo.h"

using namespace std;

class QueryInfoTable {
  public:
    vector<QueryInfo*> table;   

    QueryInfoTable(int initSize=0) {
      if (initSize) {
        table.reserve(initSize);
      }
    }

    ~QueryInfoTable() {
      for (unsigned int i=0; i<table.size(); ++i) {
        delete table[i];
      }
    }

    int add(QueryInfo* pQi) {
      table.push_back(pQi);
      return table.size() - 1;
    }

    QueryInfo* get(unsigned int i) {
      return table[i];
    }

    void finalize() {
      vector<QueryInfo*>(table).swap(table);
    }
};

#endif
