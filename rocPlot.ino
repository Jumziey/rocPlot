#include <SD.h>

#define pln(x) Serial.println(x)
#define p(x) Serial.print(x)


/*
	p("x: "); p(x); p(" y: "); p(y); p(" z: "); pln(z);
	p("zup: "); p(zup); p(" zdown: "); pln(zdown);
	
	SS -> 10
	mosi -> 11
	miso ->  12
	sck -> 13

*/

enum {
	ss = 10,
	logtime = 3*10*10*10
};

int init(byte);
File initSD(void);

float x,y,z;
float xref, yref, zref;	
float zup, zdown, zdiff; 	// init checkers 
float ac;				//Translation constant from volt to m/s^2

File logfile;
unsigned long start;


void setup() 
{
	

	Serial.begin(9600);
	
	//constants needed to be set before any call to initAcc()
	xref = yref = zref = 0;
	zup = zdown = 0;
	ac = 1;
	
	for(;;)
		if(Serial.available())
			if(initAcc(Serial.read()))
				break;
				
	pln("Accelerometer is initialized");
	
	pln();
	pln("Checking SD card");
	logfile = initSD();
	if(!logfile)
	{
		pln("SD card initialization failed, exiting program");
		delay(200); 	//So the arduino has time to empty the print buffer
		exit(1);
	}
	pln("Sd card initialized");
	
	start = millis(); //Wanna remove the initialization time. 
}

void loop() 
{
	x = (analogRead(A0)-xref)/ ac; 
	y = (analogRead(A1)-yref)/ ac;	
	z = (analogRead(A2)-zref)/ ac - 1; //<--- ASSUMING a straight orientaiton
	
	logfile.print(x); logfile.print(" "); 
	logfile.print(y); logfile.print(" "); 
	logfile.println(z)
	
	if((millis()-start) > logtime)
	{
		pln("logging done");
		logfile.close();
		delay(200); //So the arduino has time to empty the print buffer
		exit(0);
	}
}

/*##################################*/



int initAcc(byte input)
{
	switch(input)
	{
		case 'a':
			if(zup != 0 && zdown != 0) //Accelerometer allrdy initialized
				return(1);
				
			x = (analogRead(A0)-xref)/ ac; 
			y = (analogRead(A1)-yref)/ ac;	
			z = (analogRead(A2)-zref)/ ac;	
			
			if(zup == 0)
				zup = z;
			else
				zdown = z;
			
			if(zup != 0 && zdown != 0) 	//All data accumulated to initialize
			{
				ac = (fabs(zup-zdown)/2);
				xref = x; yref = y;
				if((zup-zdown)>0)
					zref = z + ac; 
				else
					zref = z - ac;
				 return(1);
			} 
		default:
			return(0);
	}
}


File initSD() 
{
	File root, entry, logfile;
	
	if(!SD.begin(ss))	//Check that the SD-card works
		return(File());
	
	root = SD.open("/", FILE_READ);
	for(;;)
	{
		entry = root.openNextFile();
		if(!entry)
			break;
		
		if(!entry.isDirectory()) //ignores directories on the sdcard
			SD.remove(entry.name());
	}
	logfile = SD.open("/log.txt", FILE_WRITE);
	return logfile;
}