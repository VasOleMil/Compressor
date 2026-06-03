#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

#include"..\src\CComp.h"
#include"CompTest.h"

//--------------------------------------------------------------------
void
CompTestStepScan()
{
	
	int Strt, Send, sn, Sser, sr, cs;

	double Tsec, Gspd; clock_t str_t, end_t;
	
	Strt = 23;    //start candidate 
	Send = 26;    //end candidate
	Sser = 2000;  //seria number per coordination number
	Gspd = 1e-3;  //growing speed

	CompInit(4);  //dimension

	sn = Strt;	while (sn <= Send)
	{		
		sr = 1; while (sr <= Sser)
		{
			srand(sr + GetTickCount());

			SortAdd(1, sn, 1.0, 0.0);

			CompLoad(1.0, 0.0, Gspd);	// GetVolume();		

			str_t = clock();
			
			while (Sc < Sn) CompStep(); //main loop
		
			end_t = clock(); cs = (Cb + Ce);

			Tsec = (double)(end_t - str_t) / CLOCKS_PER_SEC;

			GetVolume(); SortDel(1);

			printf("\n\nElapsed time: %.0f s", Tsec);
			printf("\nTotal steps: %ld", cs);
			printf("\nproductivity: %.0f", (double)cs / Tsec);
			printf("\n--------------\n");
			sr++; //repeat till seria number reached
		}	sn++; //next coordination number candidate
	}

	CompFree();
	
	return;
}
//--------------------------------------------------------------------
void
CompTestStepHold()
{
	
	int Strt, Send, sn, sr, cs; double Gspd;

	Strt = 1;    //start  
	Send = 3;    //end 
	sn = 7;      //Bn coordination number +1, expected to be > 1
	Gspd = 1e-3; //growing speed

	CompInit(2); //dimension

	srand(sn + GetTickCount());

	SortAdd(1, sn, 1.0, 0.0);

	CompLoad(1.0, 0.0, Gspd);	

	GetVolume();	

	sr = Strt;	while (sr <= Send)
	{		
		CompHold(); //main loop debug stepping
		
		cs = (Cb + Ce); sr++; 			
	}
	
	GetVolume(); 

	printf("\nTotal steps: %ld", cs);
	printf("\n--------------\n");
	
	CompFree();
	
	return;
}//simple stepping test, direct reporting.
//--------------------------------------------------------------------

