
#define pln(x) Serial.println(x)
#define p(x) Serial.print(x)

float x,y,z;
float xref, yref, zref;
float zup, zdown, zdiff;
float ac;
float temp;

void setup() 
{
	Serial.begin(9600);
	xref = yref = zref = 0;
	zup = zdown = 0;
	ac = 1;
}

void loop() 
{
start:
	
	x = (analogRead(A0)-xref)/ ac; 
	y = (analogRead(A1)-yref)/ ac;	
	z = (analogRead(A2)-zref)/ ac;	
	temp = analogRead(A5)/ ac;
	
	
	if(Serial.available() > 0) 
	{
		switch(Serial.read())
		{
			case 'a':
				if(zup != 0 && zdown != 0) 
				{
					pln("Accelerometer is allrdy initialized");
					goto start;
				}
				
			
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
				}
			
				p("x: "); p(x); p(" y: "); p(y); p(" z: "); pln(z);
				p("zup: "); p(zup); p(" zdown: "); pln(zdown);
				break;
				
			default:
				p("x: "); p(x); p(" y: "); p(y); p(" z: "); pln(z);
		}
	}
		
}