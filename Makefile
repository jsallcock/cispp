CXXFLAGS = -std=c++17

all: build/test_component build/test_camera

# component
build/test_component: build/test_component.o build/component.o
	$(CXX) $(CXXFLAGS) build/test_component.o build/component.o -o build/test_component

build/test_component.o: tests/test_component.cpp include/component.h
	$(CXX) $(CXXFLAGS) -c tests/test_component.cpp -o build/test_component.o

build/component.o: src/component.cpp include/component.h
	$(CXX) $(CXXFLAGS) -c src/component.cpp -o build/component.o


# camera
build/test_camera: build/test_camera.o build/camera.o
	$(CXX) $(CXXFLAGS) build/test_camera.o build/camera.o -o build/test_camera

build/test_camera.o: tests/test_camera.cpp include/camera.h
	$(CXX) $(CXXFLAGS) -c tests/test_camera.cpp -o build/test_camera.o

build/camera.o: src/camera.cpp include/camera.h
	$(CXX) $(CXXFLAGS) -c src/camera.cpp -o build/camera.o
