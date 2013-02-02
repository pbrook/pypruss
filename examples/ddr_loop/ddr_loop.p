.origin 0
.entrypoint START

#define PRU0_ARM_INTERRUPT 						
							
#define GPIO1 			0x4804c000		// The adress of the GPIO1 
#define GPIO_DATAOUT 	0x13c			// This is the register for setting data
#define CONST_DDR 		0x80001000		// This is where the DDR data starts
#define CTPPR_1         0x2202C 		// 

START:	
    LBCO r0, C4, 4, 4					// Load Bytes Constant Offset (?)
    CLR  r0, r0, 4						// Clear bit 4 in reg 0
    SBCO r0, C4, 4, 4					// Store Bytes Constant Offset

    MOV  r0, 0x00100000					// Configure the programmable pointer register for PRU0 by setting c31_pointer[15:0]
    MOV  r1, CTPPR_1    				// field to 0x0010. This will make C31 point to 0x80001000 (DDR memory).
    SBBO r0, r1, 0, 4					// Set the register

			MOV  r4, CONSTR_DDR					// Load r4 with the start address for the data

    LBBO r1, r4, 0, 4     				// Load values from external DDR Memory into R1
	QBEQ EXIT, r1, 0					// Exit if the counter r1 is 0
	ADD  r4, r4, 4						// Increment r4
 
SET_PINS:						
    LBBO r2, r4, 0, 4					// Load pin data into r2
    MOV  r3, GPIO1 | GPIO_DATAOUT 		// Load the address of GPIO | DATAOUT in r3
    SBBO r2, r3, 0, 4					// Set the pins

	ADD  r4, r4, 4						// r4 += 4
	LBBO r0, r4, 0, 4					// Load Delay into r0
DELAY:									
    SUB  r0, r0, 1						// Delay the required ticks
    QBNE DELAY, r0, 0					

	ADD  r4, r4, 4						// r4 += 4
    SUB  r1, r1, 1						// Decrement r1
    QBNE SET_PINS, r1, 0				// Branch back to SET_PINS if r0 != 0, abort!

EXIT:
    MOV R31.b0, PRU0_ARM_INTERRUPT+16   // Send notification to Host for program completion
HALT
