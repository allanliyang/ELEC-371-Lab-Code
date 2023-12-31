

/*-----------------------------------------------------------------*/

#ifndef _NIOS2_CONTROL_H_
#define _NIOS2_CONTROL_H_


// NIOS2_CONTOL HEADER
#define NIOS2_WRITE_STATUS(value)  (__builtin_wrctl (0, value))

#define NIOS2_READ_IENABLE()	   (__builtin_rdctl (3))

#define NIOS2_WRITE_IENABLE(value) (__builtin_wrctl (3, value))

#define NIOS2_READ_IPENDING()	   (__builtin_rdctl (4))


#endif /* _NIOS2_CONTROL_H_ */



/*-----------------------------------------------------------------*/
// TIMER HEADER
#ifndef _TIMER_H_
#define _TIMER_H_

// TIMER 1
#define TIMER1_STATUS	((volatile unsigned int *) 0x10004020)
#define TIMER1_CONTROL	((volatile unsigned int *) 0x10004024)
#define TIMER1_START_LO	((volatile unsigned int *) 0x10004028)
#define TIMER1_START_HI	((volatile unsigned int *) 0x1000402C)
#define TIMER1_SNAP_LO	((volatile unsigned int *) 0x10004030)
#define TIMER1_SNAP_HI	((volatile unsigned int *) 0x10004034)

// TIMER 2
#define TIMER2_STATUS	((volatile unsigned int *) 0x10004040)
#define TIMER2_CONTROL	((volatile unsigned int *) 0x10004044)
#define TIMER2_START_LO	((volatile unsigned int *) 0x10004048)
#define TIMER2_START_HI	((volatile unsigned int *) 0x1000404C)
#define TIMER2_SNAP_LO	((volatile unsigned int *) 0x10004050)
#define TIMER2_SNAP_HI	((volatile unsigned int *) 0x10004054)

// TIMER 3
#define TIMER3_STATUS	((volatile unsigned int *) 0x10004060)
#define TIMER3_CONTROL	((volatile unsigned int *) 0x10004064)
#define TIMER3_START_LO	((volatile unsigned int *) 0x10004068)
#define TIMER3_START_HI	((volatile unsigned int *) 0x1000406C)
#define TIMER3_SNAP_LO	((volatile unsigned int *) 0x10004070)
#define TIMER3_SNAP_HI	((volatile unsigned int *) 0x10004074)

/* define a bit pattern reflecting the position of the timeout (TO) bit
   in the timer status register */
#define TIMER_TO_BIT 0x1

#endif /* _TIMER_H_ */


/*-----------------------------------------------------------------*/
// LEDS HEADER
#ifndef _LEDS_H_
#define _LEDS_H_

/* define pointer macro for accessing the LED interface data register */

#define LEDS ((volatile unsigned int *) 0x10000010)

#endif /* _LEDS_H_ */


/*-----------------------------------------------------------------*/
// SEVEN SEGMENT DISPLAY HEADER

#define SEVEN_SEG ((volatile unsigned int*) 0x10000020)

// BIT PATTERNS FOR 0, 1, 2, 3 IN HEX DISPLAY
#define HEX0 0x3F
#define HEX1 0x6
#define HEX2 0x5B
#define HEX3 0x4F

// BIT PATTERNS FOR A, b, C, d
#define HEXA 0x77 
#define HEXB 0x7C
#define HEXC 0x39
#define HEXD 0x5E


/*-----------------------------------------------------------------*/
// JTAG UART HEADER
#define JTAG ((volatile unsigned int*) 0x10001000)
#define JTAG_STATUS ((volatile unsigned int*) 10001004)


/*-----------------------------------------------------------------*/
// SWITCHES HEADER

#define SWITCHES ((volatile unsigned int*) 0x10000040)





/*-----------------------------------------------------------------*/
/*             start of application-specific code                  */
/*-----------------------------------------------------------------*/

/* place additional #define macros here */


/* define global program variables here */
int count = 0;

int hexCount = 0;



void interrupt_handler(void)
{
	
	/* read current value in ipending register */

	/* do one or more checks for different sources using ipending value */

        /* remember to clear interrupt sources */
		
	unsigned int ipending;
	
	ipending = NIOS2_READ_IPENDING();
	
	// check timer 1 interrupt
	if (ipending & 0x4000){
		
		// CLEAR TIMER INTERRUPT
		*TIMER1_STATUS = 0;
		
		if(*LEDS == 0x3){
			*LEDS = 0x300;
		}
		else{
			*LEDS = *LEDS >> 2;
		}
	}

	// check timer 2 interrupt
	if (ipending & 0x8000){
		
		// CLEAR TIMER INTERRUPT
		*TIMER2_STATUS = 0;
		
		int switch2;
		switch2 = *SWITCHES & 0x4;
		
		//int hex_pattern = *SEVEN_SEG & 0xFF;
		
		
	
		if(switch2 == 0){
			
			if(hexCount == 3){
				*SEVEN_SEG = HEX0;
				hexCount = 0;
			}
			else if(hexCount == 0){
				*SEVEN_SEG = HEX1;
				hexCount++;
			}
			else if(hexCount == 1){
				*SEVEN_SEG = HEX2;
				hexCount++;
			}
			else if(hexCount == 2){
				*SEVEN_SEG = HEX3;
				hexCount++;
			}
		}
		else{

			if(hexCount == 3){
				*SEVEN_SEG = HEXA;
				hexCount = 0;
			}
			else if(hexCount == 0){
				*SEVEN_SEG = HEXB;
				hexCount++;
			}
			else if(hexCount == 1){
				*SEVEN_SEG = HEXC;
				hexCount++;
			}
			else if(hexCount == 2){
				*SEVEN_SEG = HEXD;
				hexCount++;
			}
		}		
	}	
		
		
	// check timer 3 interrupt	
	if (ipending & 0x10000){
		
		// CLEAR TIMER INTERRUPT
		*TIMER3_STATUS = 0;
		
		
		if(count == 0){
			*JTAG = '|';
			count = 1;
		}
		else{
			*JTAG = '-';
			count = 0;
		}
	}
}

void Init (void)
{	
	// initialize all displays to off
	*SEVEN_SEG = HEX0;
	*LEDS = 0x0;
	
	// INITIALIZE INITIAL BIT PATTERN FOR LEDS
	*LEDS = 0x300;
	
	// timers 1, 2, 3 initialization
	*TIMER1_START_HI = 0x0098;
	*TIMER1_START_LO = 0x9680;
	*TIMER1_STATUS = TIMER_TO_BIT;
	*TIMER1_CONTROL = 0x7;
	
	*TIMER2_START_HI = 0x00BE;
	*TIMER2_START_LO = 0xBC20;
	*TIMER2_STATUS = TIMER_TO_BIT;
	*TIMER2_CONTROL = 0x7;
	
	*TIMER3_START_HI = 25000000 >> 16;
	*TIMER3_START_LO = 25000000 & 0xFFFF;
	*TIMER3_STATUS = TIMER_TO_BIT;
	*TIMER3_CONTROL = 0x7;
	
	
	NIOS2_WRITE_IENABLE(0x1C000);
	
	NIOS2_WRITE_STATUS(0x1);
}


/* place additional functions here */



int main (void)
{
	Init ();	/* perform software/hardware initialization */

	while (1)
	{
		// empty for now
	}

	return 0;	/* never reached, but main() must return a value */
}

/*-----------------------------------------------------------------*/
/*              end of application-specific code                   */
/*-----------------------------------------------------------------*/



/*-----------------------------------------------------------------*/


/* 
   exception_handler.c

   This file is a portion of the original code supplied by Altera.

   It has been adapted by N. Manjikian for use in ELEC 371 laboratory work.

   Various unnecessary or extraneous elements have been excluded. For
   example, declarations in C for external functions called from asm()
   instructions are not required because any reference to external names
   in asm() instructions is embedded directly in the output written to
   the assembly-language .s file without any other checks by the C compiler.

   There is one particularly important change: on _reset_, the jump must be
   to the >> _start << location in order to properly initialize the stack
   pointer and to perform other crucial initialization tasks that ensure
   proper C semantics for variable initialization are enforced. The Altera
   version of the code jumped to main(), which will _not_ perform these
   crucial initialization tasks correctly.

   Finally, a reference to control register 'ctl4' in the asm() sequence
   has been replaced with the more meaningful alias 'ipending' for clarity.

   Other than the changes described above, the file contents have also been
   reformatted to fit in 80 columns of text, and comments have been edited.
*/


/* The assembly language code below handles processor reset */
void the_reset (void) __attribute__ ((section (".reset")));

/*****************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset" *
 * we allow the linker program to locate this code at the proper reset vector*
 * address. This code jumps to _startup_ code for C program, _not_ main().   *
 *****************************************************************************/

void the_reset (void)
{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("movia r2, _start");  /* jump to the C language _startup_ code */
  asm ("jmp r2");            /* (_not_ main, as in the original Altera file) */
}

/* The assembly language code below handles exception processing. This
 * code should not be modified; instead, the C language code in the normal
 * function interrupt_handler() [which is called from the code below]
 * can be modified as needed for a given application.
 */

void the_exception (void) __attribute__ ((section (".exceptions")));

/*****************************************************************************
 * Exceptions code. By giving the code a section attribute with the name     *
 * ".exceptions" we allow the linker program to locate this code at the      *
 * proper exceptions vector address. This code calls the interrupt handler   *
 * and later returns from the exception to the main program.                 *
 *****************************************************************************/

void the_exception (void)
{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("subi sp, sp, 128");
  asm ("stw  et, 96(sp)");
  asm ("rdctl et, ipending"); /* changed 'ctl4' to 'ipending' for clarity */
  asm ("beq  et, r0, SKIP_EA_DEC");   /* Not a hardware interrupt, */
  asm ("subi ea, ea, 4");             /* so decrement ea by one instruction */ 
  asm ("SKIP_EA_DEC:");
  asm ("stw	r1,  4(sp)"); /* Save all registers */
  asm ("stw	r2,  8(sp)");
  asm ("stw	r3,  12(sp)");
  asm ("stw	r4,  16(sp)");
  asm ("stw	r5,  20(sp)");
  asm ("stw	r6,  24(sp)");
  asm ("stw	r7,  28(sp)");
  asm ("stw	r8,  32(sp)");
  asm ("stw	r9,  36(sp)");
  asm ("stw	r10, 40(sp)");
  asm ("stw	r11, 44(sp)");
  asm ("stw	r12, 48(sp)");
  asm ("stw	r13, 52(sp)");
  asm ("stw	r14, 56(sp)");
  asm ("stw	r15, 60(sp)");
  asm ("stw	r16, 64(sp)");
  asm ("stw	r17, 68(sp)");
  asm ("stw	r18, 72(sp)");
  asm ("stw	r19, 76(sp)");
  asm ("stw	r20, 80(sp)");
  asm ("stw	r21, 84(sp)");
  asm ("stw	r22, 88(sp)");
  asm ("stw	r23, 92(sp)");
  asm ("stw	r25, 100(sp)"); /* r25 = bt (r24 = et, saved above) */
  asm ("stw	r26, 104(sp)"); /* r26 = gp */
  /* skip saving r27 because it is sp, and there is no point in saving sp */
  asm ("stw	r28, 112(sp)"); /* r28 = fp */
  asm ("stw	r29, 116(sp)"); /* r29 = ea */
  asm ("stw	r30, 120(sp)"); /* r30 = ba */
  asm ("stw	r31, 124(sp)"); /* r31 = ra */
  asm ("addi	fp,  sp, 128"); /* frame pointer adjustment */

  asm ("call	interrupt_handler"); /* call normal function */

  asm ("ldw	r1,  4(sp)"); /* Restore all registers */
  asm ("ldw	r2,  8(sp)");
  asm ("ldw	r3,  12(sp)");
  asm ("ldw	r4,  16(sp)");
  asm ("ldw	r5,  20(sp)");
  asm ("ldw	r6,  24(sp)");
  asm ("ldw	r7,  28(sp)");
  asm ("ldw	r8,  32(sp)");
  asm ("ldw	r9,  36(sp)");
  asm ("ldw	r10, 40(sp)");
  asm ("ldw	r11, 44(sp)");
  asm ("ldw	r12, 48(sp)");
  asm ("ldw	r13, 52(sp)");
  asm ("ldw	r14, 56(sp)");
  asm ("ldw	r15, 60(sp)");
  asm ("ldw	r16, 64(sp)");
  asm ("ldw	r17, 68(sp)");
  asm ("ldw	r18, 72(sp)");
  asm ("ldw	r19, 76(sp)");
  asm ("ldw	r20, 80(sp)");
  asm ("ldw	r21, 84(sp)");
  asm ("ldw	r22, 88(sp)");
  asm ("ldw	r23, 92(sp)");
  asm ("ldw	r24, 96(sp)");
  asm ("ldw	r25, 100(sp)");
  asm ("ldw	r26, 104(sp)");
  /* skip r27 because it is sp, and we did not save this on the stack */
  asm ("ldw	r28, 112(sp)");
  asm ("ldw	r29, 116(sp)");
  asm ("ldw	r30, 120(sp)");
  asm ("ldw	r31, 124(sp)");

  asm ("addi	sp,  sp, 128");

  asm ("eret"); /* return from exception */

  /* Note that the C compiler will still generate the 'standard'
     end-of-normal-function code with a normal return-from-subroutine
     instruction. But with the above eret instruction embedded
     in the final output from the compiler, that end-of-function code
     will never be executed.
   */ 
}
