/*
 * Simple TopK using heap
 * Author: tngo@walmartlabs.com
 *
 * Adapted from:
 * http://stackoverflow.com/questions/33180609/how-to-create-a-min-heap-of-fixed-size-using-stdpriority-queue
 */

#ifndef __TOPK_COMPLETE_TRIE_NODE__
#define __TOPK_COMPLETE_TRIE_NODE__

#include <algorithm>
#include <vector>
#include "CompleteTrieNode.h"

using namespace std;

class CompareCompleteTrieNode {
  public:
    bool operator() (CompleteTrieNode* const &q1, CompleteTrieNode* const &q2) {
      return q1->score > q2->score;
    }
};

class TopkCompleteTrieNode {
  public:
    unsigned int k;
    vector<CompleteTrieNode*> maxHeap;
    float minScore;

    TopkCompleteTrieNode(int k) {
      this->k = k;
      minScore = 1.e+8;
    }

    void add(CompleteTrieNode *node) {
      if (maxHeap.size() < k) {
        maxHeap.push_back(node);
        if (maxHeap.size() == k) {
          make_heap(maxHeap.begin(), maxHeap.end(), CompareCompleteTrieNode());
        }
        if (minScore > node->score) {
          minScore = node->score;
        }
      } else if (node->score > maxHeap[0]->score) {
        pop_heap(maxHeap.begin(), maxHeap.end(), CompareCompleteTrieNode());
        maxHeap.back() = node;
        push_heap(maxHeap.begin(), maxHeap.end(), CompareCompleteTrieNode());
        minScore = maxHeap[0]->score;
      }
    }

    vector<CompleteTrieNode*> * value() {
      if (maxHeap.size() < k) {
        sort(maxHeap.begin(), maxHeap.end(), CompareCompleteTrieNode());
      } else {
        sort_heap(maxHeap.begin(), maxHeap.end(), CompareCompleteTrieNode());
      }
      return new vector<CompleteTrieNode*>(maxHeap);
    }

    unsigned int size() {
      return maxHeap.size();
    }
};

#endif
