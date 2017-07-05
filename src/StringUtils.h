/*
 * Author: tngo@walmartlabs.com
 */

#ifndef __STRING_UTILS_H__
#define __STRING_UTILS_H__

#include <string>
#include <vector>

using namespace std;

vector<string> stringSplit(string &s, char delim);
string stringJoin(vector<string> &toks, char delim, int start, int stop);
vector<string> generateRotations(string &s, unsigned int max);

#endif
