.PHONY: all clean check rebuild
CXX = g++ -std=c++17
CXXFLAGS = -Wall -Werror -Wextra -g
OS = $(shell uname -s)
TEST_FLAGS = -o client_test -lgtest
SOURCE = shared_data.cc

ifeq ($(OS), Linux)
	TEST_FLAGS += -lpthread
endif

all: client tests gcov_report check

client: $(SOURCE) main.cc
	$(CXX) $(CXXFLAGS) main.cc $(SOURCE) -o client

client_debug: $(SOURCE)
	$(CXX) $(CXXFLAGS) -fsanitize=thread main.cc $(SOURCE) -o client_debug

tests: tests.cc
	$(CXX) $(CXXFLAGS) -fsanitize=thread tests.cc $(SOURCE) $(TEST_FLAGS)
	./client_test

gcov_report: tests.cc
	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage tests.cc $(SOURCE) $(TEST_FLAGS)
	./client_test
	lcov -t "test" -o test.info --no-external -c -d . --ignore-errors inconsistent
	genhtml -o report test.info --ignore-errors inconsistent
ifeq ($(OS), Darwin)
	open ./report/index.html
else
	xdg-open ./report/index.html
endif

clean:
	rm -rf *.out *.gcda *.gcno *.info report *.dSYM .DS_Store client*

check:
	clang-format -style=google -i *.cc *.h
	cppcheck *.cc
ifeq ($(OS), Darwin)
	CK_FORK=no leaks --atExit -- ./client_test
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./client_test
endif

rebuild: clean all