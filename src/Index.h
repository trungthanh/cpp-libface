/*
 * Author: tngo@walmartlabs.com
 */

#ifndef __INDEX_H__
#define __INDEX_H__

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include "macros.h"
#include "QueryInfo.h"
#include "CompleteTrie.h"
#include "StringUtils.h"
#include "QueryInfoTable.h"

#define INIT_STRING_TABLE_SIZE 3000000

using namespace std;

class Index {
  private:
    CompleteTrie *pTrie;
    QueryInfoTable* pQueryInfoTable;
    float rotationPenalty;
    unsigned int maxRotations;
  public:

    Index() {
      rotationPenalty = 0.2;
      pQueryInfoTable = new QueryInfoTable(INIT_STRING_TABLE_SIZE);
      pTrie = new CompleteTrie(pQueryInfoTable);
    }

    ~Index() {
      delete pTrie;
      delete pQueryInfoTable;
    }

    void loadFromTSVFile(string &filename) {
      ifstream file(filename.c_str());
      string line; 
      unsigned int lineNo = 0;
      while (getline(file, line)) {
        vector<string> toks = stringSplit(line, '\t');
        string query = toks[0];
        vector<string> dept_strings = stringSplit(toks[1], ',');
        vector<int> depts;
        for (unsigned int i=0; i<dept_strings.size(); ++i) {
          depts.push_back(atoi(dept_strings[i].c_str()));
        }
        float score = atof(toks[2].c_str());
 
        QueryInfo* pQI = new QueryInfo(query, depts);
        unsigned int queryIndex = pQueryInfoTable->add(pQI);
        
        pTrie->insert(query, queryIndex, score);
        float rotatedScore = score*rotationPenalty;
        vector<string> rotatedQueries = generateRotations(query, maxRotations);
        for (unsigned i=0; i<rotatedQueries.size(); ++i) {
          pTrie->insert(rotatedQueries[i], queryIndex, rotatedScore);
        }
 
        lineNo++;
        if (lineNo % 100000 == 0) {
          cout << "Loaded " << lineNo << " queries" << endl;
        }
      }
    }

    vector<CompleteTrieNode*> * topk(string &prefix, unsigned int k=8) {
      return pTrie->topk(prefix, k);
    }

    QueryInfo* getQueryInfo(int index) {
      return pQueryInfoTable->get(index);
    }
};

#endif
