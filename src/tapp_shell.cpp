#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

#include "CompleteTrie.h"
#include "Index.h"
#include "fixed_vector.h"

using namespace std;
namespace po = boost::program_options;

int processArgs(po::variables_map &vm, int argc, char* argv[]) {
  po::options_description description("Usage:");
  description.add_options()
    ("help,h", "Display this help message")
    ("indexFileType,t", po::value<string>()->default_value("tsv"), "Index file type: tsv (default), protobuf")
    ("indexFilename,f", po::value<string>(), "Index file name");
    //("topk,k", po::value<int>(), "Top k");

  //po::positional_options_description p;
  //p.add("something", 1);

  try {
    //po::store(po::command_line_parser(argc, argv).options(description).positional(p).run(), vm);
    po::store(po::command_line_parser(argc, argv).options(description).run(), vm);
    po::notify(vm);
  } catch (...) {
    cerr << description;
    return 1;
  }

  if(vm.count("help")) {
    std::cout << description;
    return 1;
  }

  if (vm["indexFilename"].as<string>() == "") {
    cout << "Error: index file name is required." << endl;
    cout << description;
    return 1;
  }

  return 0;
}

int main(int argc, char* argv[]) {
  // process arguments
  po::variables_map args;
  if (processArgs(args, argc, argv)) {
    return 1;
  }
  int k = 10;

  cout << "Loading index..." << endl;
  Index index;
  string indexFilename = args["indexFilename"].as<string>();
  index.loadFromTSVFile(indexFilename);

  string prefix;
  while (1) {
    cout << "Prefix: ";
    cin >> prefix;
    if (prefix == "exit") {
      break;
    }

    vector<CompleteTrieNode*> *results = index.topk(prefix, k);
    for (unsigned int i=0; i<results->size(); i++) {
      if ((*results)[i]->queryIndex > -1) {
        QueryInfo* pQI = index.getQueryInfo((*results)[i]->queryIndex);
        cout << "query: " << pQI->query << " score: " << (*results)[i]->score << endl;
      } else {
        cout << "something's wrong" << endl;
      }
    }
    delete results;
  }

  return 0;
}
