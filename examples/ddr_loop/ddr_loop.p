.origin 0
.entrypoint START

#define PRU0_ARM_INTERRUPT 	19
#define GPIO1 				0x4804c000		// The adress of the GPIO1 
#define GPIO_CLEARDATAOUT 	0x190
#define GPIO_SETDATAOUT 	0x194
#define GPIO_DATAOUT 		0x13c			// This is the register for setting data
#define CONST_DDR 			0x80001000
#define CTPPR_1         	0x2202C 	

START:
    LBCO r0, C4, 4, 4					// Load Bytes Constant Offset (?)
    CLR  r0, r0, 4						// Clear bit 4 in reg 0
    SBCO r0, C4, 4, 4					// Store Bytes Constant Offset

    MOV r0, 0x00100000					// Configure the programmable pointer register for PRU0 by setting c31_pointer[15:0]
    MOV r1, CTPPR_1    					// field to 0x0010. This will make C31 point to 0x80001000 (DDR memory).
    SBBO r0, r1, 0, 4

	MOV  r4, CONST_DDR
    LBBO r1, r4, 0, 4     				//Load values from external DDR Memory into R1
	QBEQ EXIT, r1, 0					// Exit if the counter r1 is 0	

BLINK:
	ADD  r4, r4, 4						// Increment r4
    LBBO r2, r4, 0, 4					// Load pin data into r2
    MOV r3, GPIO1 | GPIO_DATAOUT
    SBBO r2, r3, 0, 4

	ADD  r4, r4, 4
    LBBO r0, r4, 0, 4					// Load delay data into r0
DELAY:
    SUB r0, r0, 1
    QBNE DELAY, r0, 0

    SUB r1, r1, 1
    QBNE BLINK, r1, 0

EXIT:
    MOV R31.b0, PRU0_ARM_INTERRUPT+16   // Send notification to Host for program completion
HALT
