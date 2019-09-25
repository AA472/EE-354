#include "stdlib.h"
#include "stm32f446.h"
//extern void assemblyFunc();
void Led_out();
void moveSandUp();
void moveSandDown();
void moveWaterUp();
void moveWaterDown();
void resetHour();
void resetWater();
void resetPach();

unsigned char rands[250];
unsigned char rows [16];//= {0xFF,0xFF,0xFF,0xFF,0xFF,0x7E, 0x3C,0,0,0,0,0,0,0,0,0}; // used for output
unsigned char LEDS [16][8] ;
	//=  // this is the initial shape of the hour glass
//	{
//		{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}
//		,{2,1,1,1,1,1,1,2},{2,2,1,1,1,1,2,2},{2,2,2,0,0,2,2,2},{2,2,2,0,0,2,2,2},{2,2,0,0,0,0,2,2}
//    ,{2,0,0,0,0,0,0,2},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}
//	};
int mode = 2; //decides which of the three games 




int main()
  {
		int tmp = GPIOA_IDR & (1<<2);
		int tmp2 = GPIOA_IDR & (1<<3);
		int tmp3 = GPIOA_IDR & (1<<4);
		if(tmp !=0)
			mode = 2;
		if(tmp2 !=0)
			mode = 2;
		
		for(int i=0; i<250;i++)
			rands[i] = rand();
    int x = 0;
		//Enable Port A clock
		RCC_AHB1ENR |= 1; 
		//Enable Port C clock
    RCC_AHB1ENR |= 1<<2;  
		RCC_APB2ENR |= 0x100;  
    // A/D clock		
    GPIOA_MODER |= 0x3FF; 
		//Entire PC port is output
		GPIOC_MODER |= 0x555555555; 
		//Set Pull up pull down off
		GPIOA_PUPDR &= 0xFFFFFFFF3; 
    ADC1_CR2 |= 1; //A/D enable
		ADC1_SQR3 = 1;
		//Start conversion initial
   	ADC1_CR2 |= 1<<30; 
		if(mode == 1)
			resetHour();
		else if(mode==2)
			resetWater();


			while(1)
				{
					ADC1_CR2 |= 1<<30;//Start conversion		
					unsigned int TimeOut = 0;
					while(((ADC1_SR &(1<<1))== 0) && TimeOut < 100000)
					{
						TimeOut++;//Timeout variable in case of error
					}
					//Load in A/D value
					x = ADC1_DR;
					if(x < 1900 ){
						if(mode == 1)
							moveSandUp();
						else if (mode == 2)
							moveWaterUp();
					}
					 else if (x > 2200){
						if(mode == 1)
							moveSandDown();
						else if (mode == 2)
							moveWaterDown();
					}		
					for(int i =0; i<100; i++)
						Led_out();						 
				 
	}	
}
		//if the board is tilted, this will move the LEDS like an hour glass towards gravity.
void moveWaterUp(){
for( unsigned char  i = 1; i != 16; i++){ // go throw all rows except the bottom because it cannot go down
		if((i <16 && i > 11)||(i < 4)){
			for( unsigned char j = 0; j != 8; j++){ // go throw all the columns
					if(LEDS[i][j] == 1){
						if(LEDS[i-1][j] == 0){
							LEDS [i-1][j] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i-1]  |=  1<<j;
						}
						else if(j >0 && LEDS[i-1][j-1] == 0){
							LEDS [i-1][j-1] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i-1]  |=  1<<(j-1);
						}
							else if(j >0 && LEDS[i][j-1] == 0){
							LEDS [i][j-1] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i]  |=  1<<(j-1);
						}
					}
			}
		}
		else{
			for( unsigned char j = 7; j != 255; j--){ // go throw all the columns
				if(LEDS[i][j] == 1){
					if(LEDS[i-1][j] == 0){
						LEDS [i-1][j] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i-1]  |=  1<<j;
					}
					else if(j < 7 && LEDS[i+1][j+1] == 0 && i!=5){
						LEDS [i-1][j+1] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i-1]  |=  1<<(j+1);
					}
						else if(j < 7 && LEDS[i][j+1] == 0){
						LEDS [i][j+1] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i]  |=  1<<(j+1);
					}
				}
			}
			
		}
	}
}
void moveWaterDown(){
	for( unsigned char  i = 14; i != 255; i--){ // go throw all rows except the bottom because it cannot go down
		if((i <15 && i > 10)||(i < 4)){
			for( unsigned char j = 7; j != 255; j--){ // go throw all the columns
				if(LEDS[i][j] == 1){
					if(LEDS[i+1][j] == 0){
						LEDS [i+1][j] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i+1]  |=  1<<j;
					}
					else if(j < 7 && LEDS[i+1][j+1] == 0){
						LEDS [i+1][j+1] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i+1]  |=  1<<(j+1);
					}
						else if(j < 7 && LEDS[i][j+1] == 0 ){
						LEDS [i][j+1] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i]  |=  1<<(j+1);
					}
				}
			}
		}
		else{
			for( unsigned char j = 0; j != 8; j++){ // go throw all the columns
					if(LEDS[i][j] == 1){
						if(LEDS[i+1][j] == 0){
							LEDS [i+1][j] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i+1]  |=  1<<j;
						}
						else if(j >0 && LEDS[i+1][j-1] == 0 && i!=10){
							LEDS [i+1][j-1] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i+1]  |=  1<<(j-1);
						}
							else if(j >0 && LEDS[i][j-1] == 0){
							LEDS [i][j-1] = 1; 
							LEDS [i][j] = 0;
							rows [i] &= ~(1<<j);
							rows [i]  |=  1<<(j-1);
						}
					}
			}
		}
	}
}


	
void moveSandDown(){
		for( unsigned char  i = 14; i != 255; i--){ // go throw all rows except the bottom because it cannot go down
			for( unsigned char j = 7; j != 255; j--){ // go throw all the columns
				if(LEDS[i][j] == 1){ // if the current LED is lit up 
					if(LEDS[i + 1][j] == 0){ // if the LED under it is empty, move there
						LEDS [i+1][j] = 1; 
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i+1]  |=  1<<j;
					}
					else if(LEDS[i + 1][j + 1] == 0 && j < 7){ //otherwise check 
						LEDS [i+1][j+ 1] = 1;
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i+1]  |=  1<< (1 + j);
					}
					else if(LEDS[i + 1][j-1] == 0 && j >0){
						LEDS [i+1][j-1] = 1;
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i+1]  |=  1<<(j-1);
					}
				}
			}
		}
}	//if the board is tilted, this will move the LEDS like an hour glass towards gravity.
void moveSandUp(){
	unsigned char temp; 
		for( unsigned char  i = 1; i != 16; i++){
			temp =0;
			for( unsigned char j = 7; j != 255; j--){
				if(LEDS[i][j] == 1){
					if(LEDS[i - 1][j] == 0){
						LEDS [i-1][j] = 1;
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i-1]  |=  1<<j;
					}
					else if(LEDS[i - 1][j + 1] == 0 && j < 7){
						LEDS [i-1][j+ 1] = 1;
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i-1]  |=  1<<(j+1);
					}
					else if(LEDS[i - 1][j-1] == 0 && j >0){
						LEDS [i-1][j-1] = 1;
						LEDS [i][j] = 0;
						rows [i] &= ~(1<<j);
						rows [i-1]  |=  1<<(j-1);
					}
				}
			}
		}
}

void Led_out(void){

	unsigned int tmp = 0;
	//Store output value in tmp 
	tmp = GPIOC_ODR;

	//tmp |= 0x38;//PC3-5 = high, decoder row 0 off
	tmp |= 1<<9;//PC6 On, Decoder 1 off
	tmp &= ~(1<<8);//PC7 Off, Decoder 2 on
 
	for ( int i = 0; i < 8; i++)
	{
		
	tmp = GPIOC_ODR;
	tmp &= ~(0x1c00);//Clears decoder bits
	tmp |= i<<10;//Set i into decoder pins
	
	tmp &= ~(0xFF );//Clear driver values
	tmp |= rows[i] ;//Force value of driver
		


	tmp |= 1<<9;//PC6 On, Decoder 1 off
	tmp &= ~(1<<8);//PC7 Off, Decoder 2 on
			
	GPIOC_ODR = tmp;
		for (int k = 0; k < 100; k++);
	}
	
	for (int i = 0; i < 8; i++)
	{
	tmp = GPIOC_ODR;
	tmp &= ~(0x1c00);//Clears decoder bits
	tmp |= i<<10;//Set i into decoder pins
	
	tmp &= ~(0xFF );//Clear driver values
	tmp |= rows[i+8] ;//Force value of driver
		
	

	tmp &= ~(1<<9);//PC6 off, Decoder 1 on
	tmp |= 1<<8;//PC7 on, Decoder 2 off
			
	GPIOC_ODR = tmp;
			for (int k = 0; k < 100; k++);
		
	}	
}
void resetHour(){
	 rows[0] = 0xFF;
	 rows[1] = 0xFF;
	 rows[2] = 0xFF;
	 rows[3] = 0xFF;
	 rows[4] = 0x7E;
	 rows[5] = 0x3C;
	 rows[6] = 0;
	 rows[7] = 0;
	 rows[8] = 0;
	 rows[9] = 0;
	 rows[10] = 0;
	 rows[11] = 0;
	 rows[12] = 0;
	 rows[13] = 0;
	 rows[14] = 0;
	 rows[15] = 0;
	
	for(unsigned char i = 0; i < 16; i++){
		for(unsigned char j = 0; j<8 ; j++){
				if( i < 7) 
					LEDS[i][j] = 1;
				else 
					LEDS[i][j] = 0;
		}
	}
	 
	LEDS[5][0] = 2; LEDS [5][7] = 2;
	LEDS[6][0] = 2; LEDS [6][1] =2; LEDS[6][6] = 2; LEDS[6][7] = 2;
	LEDS[7][0] = 2; LEDS [7][1] = 2; LEDS[7][2] = 2; LEDS[7][5] = 2; LEDS [7][6] = 2; LEDS[7][7] = 2; 
	LEDS[8][0] = 2; LEDS [8][1] = 2; LEDS[8][2] = 2; LEDS[8][5] = 2; LEDS [8][6] = 2; LEDS[8][7] = 2; 
	LEDS[9][0] = 2; LEDS [9][1] =2; LEDS[9][6] = 2; LEDS[9][7] = 2;
	LEDS[10][0] = 2; LEDS [10][7] = 2;

}
void resetWater(){
		rows[0] = 0xFF;
	 rows[1] = 0xFF;
	 rows[2] = 0xFF;
	 rows[3] = 0;
	 rows[4] = 0;
	 rows[5] = 0;
	 rows[6] = 0;
	 rows[7] = 0;
	 rows[8] = 0;
	 rows[9] = 0;
	 rows[10] = 0;
	 rows[11] = 0;
	 rows[12] = 0;
	 rows[13] = 0;
	 rows[14] = 0;
	 rows[15] = 0;
	
	for(unsigned char i = 0; i < 16; i++){
		for(unsigned char j = 0; j<8 ; j++){
				if( i < 3) 
					LEDS[i][j] = 1;
				else if( i==4 && j < 7)
					LEDS[i][j]=2;
				else if( i==11 && j >0)
					LEDS[i][j]=2;
				else
					LEDS[i][j] = 0;
		}
	}
}



	