
# Source this to set up Python on Tukey

PYTHON=/home/wozniak/Public/sfw/x86/Python-2.7.6
export PYTHONUSERBASE=$HOME/sfw/x86/py
PATH=${PYTHON}/bin:${PATH}
export LD_LIBRARY_PATH=${PYTHON}/lib:/soft/compilers/gcc/4.8.1/lib:/gpfs/mira-fs0/software/x86_64/compilers/gcc/4.8.1/lib64:${LD_LIBRARY_PATH}

# CBF settings
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/Public/sfw/x86/CBFlib-0.9.3.3/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${HOME}/downloads/CBFlib-0.9.3.3/hdf5-1.8.11/src/.libs
export PYTHONPATH=${PYTHONPATH}:${HOME}/downloads/CBFlib-0.9.3.3/pycbf
