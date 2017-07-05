#include <map>
#include <stack>
#include <sstream>
#include "macros.h"
#include "TopkCompleteTrieNode.h"
#include "CompleteTrie.h"

CompleteTrie::CompleteTrie(QueryInfoTable* pQT) {
  pQueryInfoTable = pQT;
  root = new CompleteTrieNode();
}

CompleteTrie::~CompleteTrie() {
  if (this->root) {
    deleteTrieRecurse(this->root);
  }
}

void CompleteTrie::deleteTrieRecurse(CompleteTrieNode *node) {
  for (map<char, CompleteTrieNode*>::iterator it=node->children.begin();
       it!=node->children.end(); ++it) {
    if (it->second) {
      deleteTrieRecurse(it->second);
    }
  }
  delete node;
}

CompleteTrieNode* CompleteTrie::find(string &prefix, int len=0) {
  if (0 == len) {
    len = prefix.length();
  }
  CompleteTrieNode *node = this->root;
  for (int i=0; i<len; i++) {
    CompleteTrieNode *nextNode = node->getChild(prefix[i]);
    if (NULL == nextNode) {
      return NULL;
    }
    node = nextNode;
  }
  return node;
}

void CompleteTrie::insert(string &prefix, int queryIndex, float score) {
  if (prefix.length() == 0) {
    return;
  }
  int i, len = prefix.length();
  CompleteTrieNode *node = this->root;
  for (i=0; i<len; i++) {
    CompleteTrieNode *nextNode = node->getChild(prefix[i]);
    if (NULL == nextNode) {
      break;
    }
    node = nextNode;
    if (node->maxScore < score) {
      node->maxScore = score;
    }
  }
  CompleteTrieNode *nextNode;
  for (; i<len; i++) {
    nextNode = new CompleteTrieNode();
    node->children[prefix[i]] = nextNode;
    node = nextNode;
    node->maxScore = score;
  }

  if (node->queryIndex == -1) {
   node->queryIndex = queryIndex;
   node->score = score;
  } else if (node->score < score) {
    node->queryIndex = queryIndex;
    node->score = score;
  }
}

vector<CompleteTrieNode*> * CompleteTrie::topk(string &prefix, unsigned int k) {
  CompleteTrieNode *node = this->find(prefix);
  TopkCompleteTrieNode results(k);
  if (NULL == node) {
    return results.value();
  }
  stack<CompleteTrieNode*> buffer;
  buffer.push(node);
  while (buffer.size() > 0) {
    node = buffer.top();
    buffer.pop();
    if (node->queryIndex > -1 &&
        (results.size() < k || node->score > results.minScore)) {
      results.add(node);
    }
    for (map<char, CompleteTrieNode*>::iterator it=node->children.begin();
         it!=node->children.end(); ++it) {
      if (buffer.size() < MAX_BUFFER_SIZE && 
          (results.maxHeap.size() < k ||
           results.minScore < it->second->maxScore)) {
        buffer.push(it->second);
      }
    }
  }
  return results.value();
}
