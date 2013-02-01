''' ddr_write.py - test script for writing to DDR memory using the PyPRUSS library'''

import pypruss
import mmap

DDR_BASEADDR		= 0x70000000					# The actual baseaddr is 0x80000000, but due to a bug(?), 
DDR_HACK			= 0x10001000					# Python accept unsigned int as offset argument.
DDR_FILELEN			= DDR_HACK+0x1000				# The amount of memory to make available
DDR_OFFSET			= DDR_HACK						# Add the hack to the offset as well. 

with open("/dev/mem", "r+b") as f:					# Open the memory device
	ddr_mem = mmap.mmap(f.fileno(), DDR_FILELEN, offset=DDR_BASEADDR) # 

data = "".join(map(chr, [20, 0, 0, 0]))				# Make the data, it needs to be a string
ddr_mem[DDR_OFFSET:DDR_OFFSET+4] = data				# Write the data to the DDR memory, four bytes should suffice
ddr_mem.close()										# Close the memory 
f.close()											# Close the file

pypruss.modprobe()							       	# This only has to be called once pr boot
pypruss.init()										# Init the PRU
pypruss.open(0)										# Open PRU event 0 which is PRU0_ARM_INTERRUPT
pypruss.pruintc_init()								# Init the interrupt controller
pypruss.exec_program(0, "./ddr_write.bin")			# Load firmware "ddr_write.bin" on PRU 0
pypruss.wait_for_event(0)							# Wait for event 0 which is connected to PRU0_ARM_INTERRUPT
pypruss.clear_event(0)								# Clear the event
pypruss.pru_disable(0)								# Disable PRU 0, this is already done by the firmware
pypruss.exit()										# Exit, don't know what this does. 



