/*
 * Simple Query Info
 *
 * Author: tngo@walmartlabs.com
 */


#ifndef __QUERY_INFO__
#define __QUERY_INFO__

#include <string.h>
#include <string>
#include <vector>
#include "fixed_vector.h"

using namespace std;

#define MAX_DEPTS 3 // use a fixed max number of departments for now

class QueryInfo {
  public:
    char* query; // replace by string index in a string table for duplicated queries
    vector<int> depts;

    QueryInfo(string &q, vector<int> dv) {
      if (q.size()) {
        query = new char[q.size()+1];
        strcpy(query, q.c_str());
      } else {
        query = NULL;
      }
      for (unsigned int i=0; i<dv.size() && i<MAX_DEPTS; i++) {
        depts.push_back(dv[i]);
      }
      //vector<int>(this->depts).swap(this->depts);
    }

    ~QueryInfo() {
      if (query) {
        delete[] query;
      }
    }
};

#endif
