/*
 * Author: tngo@walmartlabs.com
 */

#ifndef __CONTEXT_MANAGER_H__
#define __CONTEXT_MANAGER_H__

#include <string>
#include <unordered_map>
#include <armadillo>
#include "QuerySuggestion.h"

using namespace std;

typedef unordered_map<string, unsigned int> WordTable;
typedef pair<string, unsigned int> WordIndexPair;

#define MAX_DEPTS 3 // use a fixed max number of departments for now

class ContextManager {
  public:
    long long nWords;
    unsigned int dimension;
    arma::fmat *wordVectors;
    WordTable *wordTable;

    ContextManager();
    ~ContextManager();
    int loadFromWord2Vec(const char * filename);
    arma::fcolvec getContextVector(string &context);
    //vector<int> getTopkQueries(arma::mat &contextVector, vector<QueryInfo*> &queryInfoList);
    void contextRerankHybridHeapify(string &context, QuerySuggestionList &querySuggestionList);
};

#endif
