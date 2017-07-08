# A context dependent Typeahead prototype

A Typeahead (Autocomplete) server in cpp (tapp) where queries are suggested based on a combination of global scores and local similarities with a given context. The context is some words which can be extracted from the previous query for instance. The similarity between a query and the context is the inner product between the vector representations of their bags of words. In this code, words are represented by dense vectors computed from some word-to-vec packages. Tf-idf vectors of bags of words can be used instead which can yield faster speed because of sparsity.

The http server is based on [lib-face](https://github.com/duckduckgo/cpp-libface). This Typeahead server is using a naive tree data structure with hash map links to child nodes. The space usage can be improved a little bit by using a [Ternary search tree](https://en.wikipedia.org/wiki/Ternary_search_tree).

Dependencies:
- [libuv](https://github.com/joyent/libuv/) and the [joyent http-parser](https://github.com/joyent/http-parser/) (already included in deps)
- armadillo library for matrix computation. You may need to comment out the line #define ARMA_USE_WRAPPER in your include_path/armadillo_bits/config.hpp.
- OpenBLAS for fast matrix computation with armadillo (change -L/opt/OpenBLAS/lib in Makefile to your OpenBLAS localtion)
- Boost: for command arguments (program options)
