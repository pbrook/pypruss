PyPRUSS
=======
PyPRUSS is a Python binding for controlling the 
PRUs on BeagleBone. 

To install:  
git clone https://intelligentagent@bitbucket.org/intelligentagent/pypruss.git  
cd PyPRUSS/PRUSS  
make  
make install  
export LD_LIBRARY_PATH=/usr/local/lib  
  
To try the blinkled example:  
cd ..  
cd examples/blinkled  
make  
python blinkled.py  
