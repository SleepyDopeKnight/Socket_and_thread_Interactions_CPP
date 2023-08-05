.PHONY: all clean check rebuild
CXX = g++ -std=c++17
CXXFLAGS = -Wall -Werror -Wextra -g
OS = $(shell uname -s)
TEST_FLAGS = -o server_test -lgtest
SOURCE = analysator_data.cc

ifeq ($(OS), Linux)
	TEST_FLAGS += -lpthread
endif

all: server tests gcov_report check

server: $(SOURCE) main.cc
	$(CXX) $(CXXFLAGS) main.cc $(SOURCE) -o server

# tests: tests.cc
# 	$(CXX) $(CXXFLAGS) tests.cc $(SOURCE) $(TEST_FLAGS)
# 	./server_test

# gcov_report: tests.cc
# 	$(CXX) $(CXXFLAGS) -fprofile-arcs -ftest-coverage tests.cc $(SOURCE) $(TEST_FLAGS)
# 	./server_test
# 	lcov -t "test" -o test.info --no-external -c -d .  --ignore-errors inconsistent
# 	genhtml -o report test.info --ignore-errors inconsistent
# ifeq ($(OS), Darwin)
# 	open ./report/index.html
# else
# 	xdg-open ./report/index.html
# endif

clean:
	rm -rf *.out *.gcda *.gcno *.info report *.dSYM .DS_Store server*

check:
	clang-format -style=google -i *.cc *.h
	cppcheck *.cc
# ifeq ($(OS), Darwin)
# 	CK_FORK=no leaks --atExit -- ./server_test
# else
# 	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./server_test
# endif

rebuild: clean all