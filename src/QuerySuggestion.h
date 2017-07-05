#ifndef __QUERY_SUGGESTION_H__
#define __QUERY_SUGGESTION_H__

#include <unordered_map>
#include "QueryInfo.h"
#include "Index.h"

struct QuerySuggestion {
  QueryInfo* pQueryInfo;
  float score;

  QuerySuggestion(QueryInfo *pQueryInfo, float score) {
    this->pQueryInfo = pQueryInfo;
    this->score = score;
  }
};

class CompareQuerySuggestion {
  public:
    bool operator() (QuerySuggestion const &q1, QuerySuggestion const &q2) {
      return q1.score < q2.score;
    }
};

typedef vector<QuerySuggestion> QuerySuggestionList;
typedef unordered_map<string, QuerySuggestionList> QuerySuggestionCache;

#endif
