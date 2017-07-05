/*
 * Simple Complete Trie Node
 *
 * Author: tngo@walmartlabs.com
 */

#ifndef __COMPLETE_TRIE_NODE__
#define __COMPLETE_TRIE_NODE__

#include <map>
#include <string>
#include <vector>

using namespace std;

class CompleteTrieNode {
  public:
    map<char, CompleteTrieNode*> children;
    int queryIndex;
    float score;
    float maxScore;

    CompleteTrieNode(int qi=-1, float score=-1) {
      queryIndex = qi;
      if (qi > -1) {
        maxScore = score;
        score = score;
      } else {
        maxScore = -1;
        score = score;
      }
    }

    void addChild(char key, CompleteTrieNode* p) {
      children[key] = p;
    }

    CompleteTrieNode* getChild(char key) {
      if (children.find(key) != children.end()) {
        return children[key];
      }
      return NULL;
    }
};

#endif
