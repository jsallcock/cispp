CIS++: Modelling tools for Coherence Imaging Spectroscopy (CIS) of plasmas.


This project is a port of the Python library [PyCIS](https://github.com/jsallcock/pycis) to C++. I started this Summer 2022, mostly to improve my C++ skills. The code may find use someday due to improved performance. Current status: untested work-in-progress.


Requires:
- [Eigen](https://gitlab.com/libeigen/eigen): Linear algebra library used for Mueller matrix model.
- [yaml-cpp](https://github.com/jbeder/yaml-cpp): .YAML config file parsing.
- [FFTW](https://github.com/FFTW/fftw3): Fast Fourier transforms used in demodulation of data (not implemented yet).
