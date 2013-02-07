import pypruss

print "Running modprobe"
pypruss.modprobe(1000)
print "modprobe ok"
pypruss.init()	
print "init ok"
pypruss.open(0)
print "open ok"
pypruss.pruintc_init()	
print "intc ok"
print "ddr addr is "+hex(pypruss.ddr_addr())
print "ddr size is "+hex(pypruss.ddr_size())
pypruss.exit()
print "exit ok"
pypruss.modunprobe()
print "modunprobe ok"
