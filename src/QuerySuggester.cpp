#include <string>
#include "QuerySuggester.h"
#include "CompleteTrieNode.h"
#include "ContextManager.h"


#define JSON_RESERVE_SIZE 1024


QuerySuggester::QuerySuggester(Index *pIndex, ContextManager *pContextManager) {
  this->pIndex = pIndex;
  this->pContextManager = pContextManager;
}

QuerySuggestionList QuerySuggester::suggest(
    string &prefix,
    unsigned int k,
    string &context,
    string &method) {

  // no context, check cached results first, then call suggester
  if (context == "") {
    if (this->cache.find(prefix) != this->cache.end()) {
      cerr << "cache hit for prefix " << prefix << endl;
      return this->cache[prefix];
    }

    return this->suggestWithoutContext(prefix, k, true);
  }

  // have context
  cout << "context: " << context << endl;
  return this->suggestWithContextHybrid(prefix, k, context);
}

QuerySuggestionList QuerySuggester::suggestWithoutContext(
    string &prefix,
    unsigned int k,
    bool doCache=true) {
  QuerySuggestionList sl;
  vector<CompleteTrieNode*> *results = pIndex->topk(prefix, k);
  for (unsigned int i=0; i<results->size(); i++) {
    if ((*results)[i]->queryIndex > -1) {
      QueryInfo* pQI = pIndex->getQueryInfo((*results)[i]->queryIndex);
      QuerySuggestion suggestion(pQI, (*results)[i]->score);
      //cerr << "query: " << pQI->query << " - score: " << (*results)[i]->score << endl;
      sl.push_back(suggestion);
    } else {
      cerr << "something's wrong" << endl;
    }
  }
  delete results;
  if (doCache) {
    this->cache[prefix] = sl;
  }
  return sl;
}

QuerySuggestionList QuerySuggester::suggestWithContextHybrid(
    string &prefix,
    unsigned int k,
    string &context) {
  QuerySuggestionList suggestionList = this->suggestWithoutContext(prefix, k*16, false);
  pContextManager->contextRerankHybridHeapify(context, suggestionList);
  QuerySuggestionList reranked;
  unsigned int slSize = suggestionList.size();
  for (unsigned int i=0; i<k && i<slSize; i++) {
    reranked.push_back(suggestionList[0]);
    //cout << "score: " << suggestionList[0].score << endl;
    suggestionList.pop_back();
    pop_heap(suggestionList.begin(), suggestionList.end(), CompareQuerySuggestion());
  }
  return reranked;
}

string querySuggestionListToJSON(QuerySuggestionList &sl) {
  string json = "{\"specific\":[";
  json.reserve(JSON_RESERVE_SIZE);
  for (unsigned int i=0; i<sl.size(); ++i) {
    string query(sl[i].pQueryInfo->query);
    json += "{\"query\":\"" + query + "\",";
    json += "\"bold_ranges\":[],";
    json += "\"super_deps\":[";
    for (unsigned int di=0; di<sl[i].pQueryInfo->depts.size(); ++di) {
      string dept = std::to_string((long long unsigned int)sl[i].pQueryInfo->depts[di]);
      json += dept;
      if (di < sl[i].pQueryInfo->depts.size() - 1) {
        json += ",";
      }
    }
    json += "]}"; 
    if (i < sl.size() - 1) {
      json += ",";
    }
  }
  json += "]}";
  return json;
}
