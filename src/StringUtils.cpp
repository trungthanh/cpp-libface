#include <sstream>
#include "StringUtils.h"

vector<string> stringSplit(string &s, char delim=' ') {
  string tok;
  vector<string> toks;
  stringstream ss(s);
  while (getline(ss, tok, delim)) {
    toks.push_back(tok);
  }
  return toks;
}

string stringJoin(vector<string> &toks, char delim, int start, int stop) {
  stringstream ss;
  for (int i=start; i<=stop; ++i) {
    if (i != start) {
      ss << delim;
    }
    ss << toks[i];
  }
  string result = ss.str();
  return result;
}

vector<string> generateRotations(string &s, unsigned int max) {
  vector<string> results;
  vector<string> toks = stringSplit(s, ' ');
  unsigned int end = toks.size() - 1;
  for (unsigned int i=1; i<toks.size() && i<=max; ++i) {
    results.push_back(
      stringJoin(toks, ' ', i, end) +
      " " +
      stringJoin(toks, ' ', 0, i-1)
    );
  }
  return results;
}
