		
		AREA MyProg, CODE, READONLY
	  EXPORT assemblyFunc
ODR EQU 0x40020814

assemblyFunc PROC
	
	
	push {R0-R5};


	ldr R1, =ODR;
	
	 
	mov R2,R1;
	

	orr R2,#1;
	
	str R2,[R1];
	mov R0,#2000;
	

Delay subs R0, #1;
	cmp R0, #0;
	BNE Delay;
	
	
	subs R2, #1;
	

	str R2, [R1]; 
	

	pop {R0-R5};
	ENDP
 END