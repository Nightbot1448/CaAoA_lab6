SRC_PATH = ./src/
TEST_PATH = ./tests/

GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

G++ = g++
G++_FLAGS = -c -I $(GOOGLE_TEST_INCLUDE) -std=c++11
LD_FLAGS = -L /usr/local/lib -l $(GOOGLE_TEST_LIB) -l pthread -std=c++11

AC_OBJECTS = Aho_Corasick_src.o Aho_Corasick.o
SWJ_OBJECTS = search_with_joker.o search_with_joker_src.o
T_OBJECTS = Aho_Corasick_src.o test_cases.o search_with_joker_src.o

all : Aho_Corasick search_with_joker test_Aho_Corasick test_Joker

Aho_Corasick : $(AC_OBJECTS)
	$(G++) $(AC_OBJECTS) -o $@ $(LD_FLAGS)

search_with_joker : $(SWJ_OBJECTS) 
	$(G++) $(SWJ_OBJECTS) -o $@ $(LD_FLAGS)

test_Aho_Corasick : test_Aho_Corasick.o $(T_OBJECTS)
	$(G++) $^ -o $@ $(LD_FLAGS)

test_Joker : test_search_with_joker.o $(T_OBJECTS)
	$(G++) $^ -o $@ $(LD_FLAGS)

testAll : test_Aho_Corasick test_Joker
	./test_Aho_Corasick
	./test_Joker
%.o : $(SRC_PATH)%.cpp
	$(G++) $^ -o $@ $(G++_FLAGS)

%.o : $(TEST_PATH)%.cpp
	$(G++) $^ -o $@ $(G++_FLAGS)

clean:
	rm -rf *.o

cleanAll: clean
	rm -rf Aho_Corasick search_with_joker test_Aho_Corasick test_Joker