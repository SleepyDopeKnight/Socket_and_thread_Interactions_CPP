.PHONY: all clean code_check memory_check rebuild
CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Werror -Wextra -g
OS = $(shell uname -s)
TEST_FLAGS ?= -o server_test.out -lgtest
SOURCE = server

ifeq ($(OS), Linux)
	TEST_FLAGS += -lpthread
endif

all: $(SOURCE).a $(SOURCE).out $(SOURCE)_test.out report memory_check code_check

$(SOURCE).a: $(SOURCE).cc
	$(CXX) $(CXXFLAGS) $(SOURCE).cc -c
	ar rcs $(SOURCE).a $(SOURCE).o
	ranlib $(SOURCE).a

$(SOURCE).out: $(SOURCE).a main.cc
	$(CXX) $(CXXFLAGS) main.cc $(SOURCE).a -o $(SOURCE).out

$(SOURCE)_test.out: $(SOURCE).a tests.cc
	$(CXX) $(CXXFLAGS) -fsanitize=thread tests.cc $(SOURCE).a $(TEST_FLAGS)
	./$(SOURCE)_test.out

report: $(SOURCE).a tests.cc
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage tests.cc $(SOURCE).a $(TEST_FLAGS)
	./$(SOURCE)_test.out
	lcov -t "test" -o test.info --no-external -c -d . --ignore-errors inconsistent
	genhtml -o report test.info --ignore-errors inconsistent

clean:
	rm -rf *.out *.gcda *.gcno *.info report *.dSYM .DS_Store *.a *.o

code_check:
	clang-format -style=google -i *.cc *.h
	cppcheck *.cc

memory_check:
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./$(SOURCE)_test.out
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./$(SOURCE)_test.out
endif

rebuild: clean all