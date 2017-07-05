#ifndef __QUERY_SUGGESTER_H__
#define __QUERY_SUGGESTER_H__

#include <unordered_map>
#include "QuerySuggestion.h"
#include "Index.h"
#include "ContextManager.h"

typedef unordered_map<string, QuerySuggestionList> QuerySuggestionCache;

class QuerySuggester {
  private:
    Index *pIndex;
    ContextManager *pContextManager;

    QuerySuggestionCache cache;
    QuerySuggestionList suggestWithoutContext(
      string &prefix,
      unsigned int k,
      bool doCache);
    QuerySuggestionList suggestWithContextHybrid(
      string &prefix,
      unsigned int k,
      string &context);
  public:
    QuerySuggester(Index *pIndex, ContextManager *pContextManager);
    QuerySuggestionList suggest(
      string &prefix,
      unsigned int k,
      string &context,
      string &method);
};

string querySuggestionListToJSON(QuerySuggestionList &sl);

#endif
