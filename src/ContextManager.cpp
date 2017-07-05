#include <stdio.h>
#include <math.h>
#include "macros.h"
#include "StringUtils.h"
#include "ContextManager.h"

#define WORD_BUFFER_SIZE 256
#define EXP2 7.3891
#define ALPHA 0.1
#define BETA 0.9
#define RELEVANCE_THRESHOLD 0.4

static float vectorNorm(arma::fcolvec &vec) {
  float norm = 0.0;
  for (unsigned int i=0; i<vec.n_rows; ++i) {
    norm += vec(i)*vec(i);
  }
  return sqrt(norm);
}

void normalize(arma::fcolvec &vec) {
  float norm = vectorNorm(vec);
  if (norm == 0.0) {
    return;
  }
  for (unsigned int i=0; i<vec.n_rows; ++i) {
    vec(i) = vec(i)/norm;
  }
}

ContextManager::ContextManager() {
  this->wordVectors = NULL;
  this->wordTable = NULL;
}

ContextManager::~ContextManager() {
  _safeDelete(wordVectors); 
  _safeDelete(wordTable);
}

int ContextManager::loadFromWord2Vec(const char * filename) {
  FILE *f = fopen(filename, "rb");
  if (f == NULL) {
    return -1;
  }

  long long words, size;
  fscanf(f, "%lld", &words);
  fscanf(f, "%lld", &size);

  this->nWords = words;
  this->dimension = size;
  this->wordVectors = new arma::fmat(size, words);
  this->wordTable = new WordTable();

  if (this->wordVectors == NULL) {
    return -1; // not enough memory
  }

  long long b, a;
  char buf[WORD_BUFFER_SIZE];
  float tempValue, norm;
  char tempChar;
  for (b=0; b<words; b++) {
    // get the word
    a = 0;
    while (1) {
      buf[a] = fgetc(f);
      if (feof(f) || buf[a] == ' ') {
        break;
      }
      if (a < WORD_BUFFER_SIZE && buf[a] != '\n') {
        a++;
      }
    }
    buf[a] = 0;
    this->wordTable->insert(make_pair<string, unsigned int>(string(buf), b));

    // get word vector
    for (a = 0; a < size; a++) {
      //fread(&M[a + b * size], sizeof(float), 1, f);
      fscanf(f, "%f", &tempValue);
      (*this->wordVectors)(a, b) = tempValue;
    }

    // get the new line
    fscanf(f, "%c", &tempChar);

    // normalize word vector
    norm = 0.0;
    for (a = 0; a < size; a++) {
      norm += (*this->wordVectors)(a, b)*(*this->wordVectors)(a, b);
    }
    norm = sqrt(norm);
    for (a = 0; a < size; a++) {
      (*this->wordVectors)(a, b) = (*this->wordVectors)(a,b)/norm;
    }
  }

  fclose(f);

  return 0;
}

/*
void ContextManager::contextRerankHybridHeapify(string &context, QuerySuggestionList &querySuggestionList) {
  arma::fcolvec contextVector = this->getContextVector(context);
  if (vectorNorm(contextVector) == 0.0) {
    return;
  }
  arma::fcolvec queryVec;
  float contextScore;
  unsigned int i;
  for (i=0; i<querySuggestionList.size(); ++i) {
    string query(querySuggestionList[i].pQueryInfo->query);
    queryVec = this->getContextVector(query);
    contextScore = arma::dot(queryVec.t(), contextVector);
    //if (i<20) {
    //  cout << "query: " << query << ", score: " << querySuggestionList[i].score << ", context score: " << contextScore << endl;
    //}
    querySuggestionList[i].score = querySuggestionList[i].score * exp(2*contextScore)/exp2;
  }
  make_heap(querySuggestionList.begin(), querySuggestionList.end(), CompareQuerySuggestion());
}
*/

void ContextManager::contextRerankHybridHeapify(string &context, QuerySuggestionList &querySuggestionList) {
  // get context vector
  arma::fcolvec contextVector = this->getContextVector(context);
  if (vectorNorm(contextVector) == 0.0) {
    return;
  }

  // get query semantic matrix
  unsigned int i;
  arma::fmat queryMatrix(this->dimension, querySuggestionList.size());
  for (i=0; i<querySuggestionList.size(); ++i) {
    string query(querySuggestionList[i].pQueryInfo->query);
    queryMatrix.col(i) = this->getContextVector(query);
  }

  arma::Row<float> contextScores(querySuggestionList.size());
  contextScores = contextVector.t() * queryMatrix;
  for (i=0; i<querySuggestionList.size(); ++i) {
#ifdef DEBUG
    if (i<20) {
      string query(querySuggestionList[i].pQueryInfo->query);
      cout << "- query: " << query
        << ", score: " << querySuggestionList[i].score
        << ", context score: " << contextScores(i)
        << ", new score: " << ALPHA*log(querySuggestionList[i].score) + (contextScores(i) > RELEVANCE_THRESHOLD ? BETA*log(1+contextScores(i)) : 0)
        << endl;
    }
#endif
    querySuggestionList[i].score = ALPHA*log(querySuggestionList[i].score) + (contextScores(i) > RELEVANCE_THRESHOLD ? BETA*log(1+contextScores(i)) : 0);
  }
  make_heap(querySuggestionList.begin(), querySuggestionList.end(), CompareQuerySuggestion());
}


arma::fcolvec ContextManager::getContextVector(string &context) {
  vector<string> words = stringSplit(context, ' ');
  arma::fcolvec contextVector(this->dimension);
  unsigned int i;
  for (i=0; i<this->dimension; ++i) {
    contextVector(i) = 0.0;
  }
  for (i=0; i<words.size(); ++i) {
    if (this->wordTable->find(words[i]) != this->wordTable->end()) {
      contextVector += this->wordVectors->col(this->wordTable->at(words[i]));
    }
  }
  if (words.size() > 1) {
    normalize(contextVector);
  }
  return contextVector;
}
