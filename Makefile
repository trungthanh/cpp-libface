CXXFLAGS=       -std=c++0x -Wall $(COPT) -D_FILE_OFFSET_BITS=64 -DARMA_USE_BLAS -L/opt/OpenBLAS/lib
LINKFLAGS=	-lm -lrt -pthread -larmadillo -lopenblas
INCDEPS=        include/segtree.hpp include/sparsetable.hpp include/benderrmq.hpp \
                include/phrase_map.hpp include/suggest.hpp include/types.hpp \
                include/utils.hpp include/httpserver.hpp
INCDIRS=        -I . -I deps
OBJDEPS=        src/httpserver.o deps/libuv/libuv.a
HTTPSERVERDEPS= src/httpserver.cpp include/httpserver.hpp include/utils.hpp \
		include/types.hpp

ifeq "$(findstring debug,$(MAKECMDGOALS))" ""
OBJDEPS += deps/http-parser/http_parser.o
else
OBJDEPS += deps/http-parser/http_parser_g.o
endif

.PHONY: all clean debug test perf

all: CXXFLAGS += -O3
all: targets

debug: CXXFLAGS += -g -DDEBUG
debug: targets

test: CXXFLAGS += -g -DDEBUG
perf: CXXFLAGS += -O3

targets: tapp

tapp: src/main.cpp src/CompleteTrie.cpp src/QuerySuggester.cpp src/StringUtils.cpp src/ContextManager.cpp $(OBJDEPS) $(INCDEPS)
	$(CXX) -o tapp src/main.cpp src/CompleteTrie.cpp src/QuerySuggester.cpp src/StringUtils.cpp src/ContextManager.cpp $(OBJDEPS) $(INCDIRS) $(CXXFLAGS) $(LINKFLAGS)

tapp-shell:
	$(CXX) -o tapp-shell src/QuerySuggester.cpp src/CompleteTrie.cpp src/ContextManager.cpp src/StringUtils.cpp src/tapp_shell.cpp $(INCDIRS) $(CXXFLAGS) $(LINKFLAGS) -lboost_program_options -DDEBUG

src/httpserver.o: $(HTTPSERVERDEPS)
	$(CXX) -o src/httpserver.o -c src/httpserver.cpp $(INCDIRS) $(CXXFLAGS)

deps/libuv/libuv.a:
	$(MAKE) -C deps/libuv

deps/http-parser/http_parser_g.o:
	$(MAKE) -C deps/http-parser http_parser_g.o

deps/http-parser/http_parser.o:
	$(MAKE) -C deps/http-parser http_parser.o

test:
	$(CXX) -o tests/containers tests/containers.cpp -I . $(CXXFLAGS)
	tests/containers

perf:
	$(CXX) -o tests/rmq_perf tests/rmq_perf.cpp -I . $(CXXFLAGS)
	tests/rmq_perf

clean:
	$(MAKE) -C deps/libuv clean
	$(MAKE) -C deps/http-parser clean
	rm -f tapp tapp-shell tests/containers tests/rmq_perf src/httpserver.o
