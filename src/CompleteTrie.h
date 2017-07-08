/*
 * Simple Complete Trie
 *
 * Author: tngo@walmartlabs.com
 */

#ifndef __COMPLETE_TRIE__
#define __COMPLETE_TRIE__

#include <vector>
#include <string>
#include "CompleteTrieNode.h"
#include "QueryInfoTable.h"

#define MAX_BUFFER_SIZE 128

using namespace std;

// This code is currently implementing a tree instead of a trie
class CompleteTrie {
  private:
    CompleteTrieNode *root;
    QueryInfoTable *pQueryInfoTable;
  public:
    CompleteTrie(QueryInfoTable* pQueryInfoTable);
    ~CompleteTrie();
    void deleteTrieRecurse(CompleteTrieNode *node);

    CompleteTrieNode* find(string &prefix, int len);
    void insert(string &prefix, int queryIndex, float score);
    vector<CompleteTrieNode*>* topk(string &prefix, unsigned int k);
};

#endif
