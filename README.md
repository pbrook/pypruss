PyPRUSS
=======
PyPRUSS is a Python binding for controlling the 
PRUs on BeagleBone. 

For examples and inspiration: [http://hipstercircuits.com/?cat=5](http://hipstercircuits.com/?cat=5)  
 
To install:  
----------
    git clone https://intelligentagent@bitbucket.org/intelligentagent/pypruss.git  
    cd PyPRUSS
    python setup.py install
    export LD_LIBRARY_PATH=/usr/local/lib  
  
Note that you must load the uio_pruss kernel module. There is a function for 
loading and unloading this in the library called modprobe() with an optional 
argument for the DDR size. To do it manually its "modprobe uio_pruss". This must be done 
on every boot. 

To try the blinkled example:  
----------------------------
    cd PyPRUSS/examples/blinkled  
    make  
    python blinkled.py  

You should then see three of the user leds blink 10 times. 

There are a few other examples in there as well, have a look at them for other functions. 
