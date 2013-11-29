# Kevin Depue / Lucas Ellis (2013)
# opC++ Makefile.

# Make sure that you have the ${BOOST} environment variable
# defined to the boost implementation on your platform!

# environment variables
BOOST = /usr/local/Cellar/boost/1.55.0
BOOST_INC = ${BOOST}/include/
BOOST_LIB = ${BOOST}/lib/

# flags
FLAGS = -DOPCPP

# headers
INC =
INC += -I ${BOOST_INC}
INC += -I ../inc/
INC += -I ../src/

# source
SRC =
SRC += ../src/opcpp/opcpp_source.cpp
SRC += ../src/opcpp/regex_source.cpp

# debug
debug:
	clang++ -g -O0 ${FLAGS} ${INC} -o opcpp ${SRC} -lboost_filesystem

# release
release:
	clang++ -O0 ${FLAGS} ${INC} -o opcpp ${SRC} -lboost_filesystem

clean:
	rm -fr *.o opcpp
