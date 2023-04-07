CIS++: Modelling tools for Coherence Imaging Spectroscopy (CIS) of plasmas.


This project is a port of the Python library [PyCIS](https://github.com/jsallcock/pycis) to C++. I started this Summer 2022 as a learning exercise. Current status: untested work-in-progress.


Requires:
- [Eigen](https://gitlab.com/libeigen/eigen): Linear algebra library used for Mueller matrix model.
- [yaml-cpp](https://github.com/jbeder/yaml-cpp): .YAML config file parsing.
- [FFTW](https://github.com/FFTW/fftw3): Fast Fourier transforms used in demodulation of data (not implemented yet).

Build:
- `$ cmake -S . -B build`
- `$ cd build`
- `$ make`
- Set environment variable `CISPP_ROOT` to point to the project root.

TODO:
- Output images to HDF5
- Output images to a real image format (not .pbm)
- Parallelise
- Python bindings using [pybind11](https://github.com/pybind/pybind11)
