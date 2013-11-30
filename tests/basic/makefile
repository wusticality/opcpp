# variables
OPCPP = ../../build/opcpp
PATHS = -d "../../distribution/opcpp/dialects/","." -gd "generated"
DOH = -doh "opc++dialect.doh"
OH = -oh "test.oh"
FLAGS = -globmode -verbose

opcpp:
	${OPCPP} ${PATHS} ${DOH} ${OH} ${FLAGS}

debug: opcpp
	clang++ -g -v -O0 -o test generated/Generated.ocppindex main.cpp

clean:
	rm -fr generated test test.dSYM
