#include "CompleteTrie.h"
#include "Index.h"
#include "fixed_vector.h"

using namespace std;
namespace po = boost::program_options;

int initIndex(string &indexFilename) {
  cout << "Loading index..." << endl;
  Index index;
  index.loadFromTSVFile(indexFilename);
}
