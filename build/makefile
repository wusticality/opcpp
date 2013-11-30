# Kevin Depue / Lucas Ellis (2013)
# opC++ Makefile.

# Make sure that you have the ${BOOST} environment variable
# defined to the boost implementation on your platform!

# environment variables
BOOST = /usr/local/Cellar/boost/1.55.0
BOOST_INC = ${BOOST}/include/
BOOST_LIB = ${BOOST}/lib/

# flags
FLAGS = -ftemplate-depth=512

# headers
INC =
INC += -I ${BOOST_INC}
INC += -I ../inc/
INC += -I ../src/

# source
SRC =
SRC += ../src/opcpp/opcpp_source.cpp
SRC += ../src/opcpp/regex_source.cpp
SRC += ../src/opcpp/linux_source.cpp

# libraries
LIB =
LIB += -L ${BOOST_LIB}

# linking
LINK = -lboost_filesystem -lboost_system

# debug
debug:
	clang++ -g -v -O0 ${FLAGS} ${INC} ${LIB} -o opcpp ${SRC} ${LINK}

# release
release:
	clang++ -O0 ${FLAGS} ${INC} ${LIB} -o opcpp ${SRC} ${LINK}

clean:
	rm -fr *.o opcpp
