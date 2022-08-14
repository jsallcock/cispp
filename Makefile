build/out: build/out.o
	$(CXX) build/out.o -o build/out -std=c++17

build/out.o: src/component.cpp include/component.h
	$(CXX) -c src/component.cpp -o build/out.o -std=c++17