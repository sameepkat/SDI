CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
LDFLAGS =
TEST_LDFLAGS = -lgtest -lgtest_main -pthread

TARGET = sdi
TEST_TARGET = test_runner

# Main program sources
SRCS = main.cpp lexer.cpp parser.cpp differentiator.cpp
OBJS = $(SRCS:.cpp=.o)

# Library sources (excluding main.cpp for tests)
LIB_SRCS = lexer.cpp parser.cpp differentiator.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

# Test sources
TEST_SRCS = $(wildcard tests/*.cpp)
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Test target
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(LIB_OBJS) $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(LIB_OBJS) $(TEST_OBJS) $(TEST_LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests/%.o: tests/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
