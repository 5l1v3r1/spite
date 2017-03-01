MPICXX=mpicxx

CFLAGS=-O3 --std=c++11
LDFLAGS=

CPPSOURCES=MainDriver.cpp SyntheticTask.cpp

OFILES = ${CPPSOURCES:.cpp=.o}

all: SPTE_Proxy

SPTE_Proxy: ${OFILES}
	${MPICXX} ${LDFLAGS} ${OFILES} -o SPTE_Proxy

%.o: %.cpp
	${MPICXX} ${CFLAGS} -c $< -o $@

clean:
	rm -f ./*.o ./SPTE_Proxy
