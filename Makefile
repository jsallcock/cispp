CXXFLAGS:=-std=c++17
CXXFLAGS+=-O2
CXXFLAGS+=-Wall
CXXFLAGS+=-I.
CXXFLAGS+=-mmacosx-version-min=12.5


all: build/test_math \
	 build/test_spectrum \
	 build/test_coherence \
	 build/test_material \
	 build/test_component \
	 build/test_camera \
	 build/test_instrument


# MATH
build/test_math: build/test_math.o build/math.o
	$(CXX) $(CXXFLAGS) build/test_math.o build/math.o -o build/test_math

build/test_math.o: test/test_math.cpp include/math.h
	$(CXX) $(CXXFLAGS) -c test/test_math.cpp -o build/test_math.o

build/math.o: src/math.cpp include/math.h
	$(CXX) $(CXXFLAGS) -c src/math.cpp -o build/math.o


# SPECTRUM
build/test_spectrum: build/test_spectrum.o build/spectrum.o
	$(CXX) $(CXXFLAGS) build/test_spectrum.o build/spectrum.o -o build/test_spectrum

build/test_spectrum.o: test/test_spectrum.cpp include/spectrum.h
	$(CXX) $(CXXFLAGS) -c test/test_spectrum.cpp -o build/test_spectrum.o

build/spectrum.o: src/spectrum.cpp include/spectrum.h
	$(CXX) $(CXXFLAGS) -c src/spectrum.cpp -o build/spectrum.o


# COHERENCE
build/test_coherence: build/test_coherence.o build/coherence.o build/spectrum.o build/math.o
	$(CXX) $(CXXFLAGS) build/test_coherence.o build/coherence.o build/spectrum.o build/math.o -o build/test_coherence

build/test_coherence.o: test/test_coherence.cpp include/coherence.h
	$(CXX) $(CXXFLAGS) -c test/test_coherence.cpp -o build/test_coherence.o

build/coherence.o: src/coherence.cpp include/coherence.h
	$(CXX) $(CXXFLAGS) -c src/coherence.cpp -o build/coherence.o


# MATERIAL
build/test_material: build/test_material.o build/material.o
	$(CXX) $(CXXFLAGS) build/test_material.o build/material.o -o build/test_material -lyaml-cpp

build/test_material.o: test/test_material.cpp include/material.h
	$(CXX) $(CXXFLAGS) -c test/test_material.cpp -o build/test_material.o

build/material.o: src/material.cpp include/material.h
	$(CXX) $(CXXFLAGS) -c src/material.cpp -o build/material.o


# COMPONENT
build/test_component: build/test_component.o build/component.o build/material.o
	$(CXX) $(CXXFLAGS) build/test_component.o build/component.o build/material.o -o build/test_component -lyaml-cpp

build/test_component.o: test/test_component.cpp include/component.h
	$(CXX) $(CXXFLAGS) -c test/test_component.cpp -o build/test_component.o

build/component.o: src/component.cpp include/component.h
	$(CXX) $(CXXFLAGS) -c src/component.cpp -o build/component.o


# CAMERA
build/test_camera: build/test_camera.o build/camera.o build/component.o build/material.o
	$(CXX) $(CXXFLAGS) build/test_camera.o build/camera.o build/component.o build/material.o \
	-o build/test_camera -lyaml-cpp

build/test_camera.o: test/test_camera.cpp include/camera.h
	$(CXX) $(CXXFLAGS) -c test/test_camera.cpp -o build/test_camera.o

build/camera.o: src/camera.cpp include/camera.h
	$(CXX) $(CXXFLAGS) -c src/camera.cpp -o build/camera.o


# INSTRUMENT
build/test_instrument: build/test_instrument.o build/instrument.o build/component.o build/camera.o build/material.o
	$(CXX) $(CXXFLAGS) build/test_instrument.o build/instrument.o build/component.o build/camera.o build/material.o \
	-o build/test_instrument -lyaml-cpp

build/test_instrument.o: test/test_instrument.cpp include/instrument.h
	$(CXX) $(CXXFLAGS) -c test/test_instrument.cpp -o build/test_instrument.o

build/instrument.o: src/instrument.cpp include/instrument.h
	$(CXX) $(CXXFLAGS) -c src/instrument.cpp -o build/instrument.o
