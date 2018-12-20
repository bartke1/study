%.o: %.cpp *.hpp
	$(CXX) -o $@ $*.cpp -g -c

all: main.o sut.o foo_library.o
	$(CXX) main.o sut.o foo_library.o -o main

all_ut: main_ut.o sut.o foo_library_mock.o
	$(CXX) main_ut.o sut.o foo_library_mock.o -lgtest -lgmock -lpthread -o main_ut

run: all
	./main

run_ut: all_ut
	./main_ut

clean:
	rm -f *.o
	rm -f main main_ut