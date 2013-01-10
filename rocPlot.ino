#include <SD.h>
#include <Wire.h>

/*

A5 - SCL
A4 - SDA

*/

#define pln(x) Serial.println(x)
#define p(x) Serial.print(x)

enum {
	gyro_dev = (0xD0>>1), //Removing the r/w bit

	dlpf_cfg_0 = 1<<0,
	dlpf_cfg_1 = 1<<1,
	dlpf_cfg_2 = 1<<2,
	dlpf_fs_sel_0 = 1<<3,
	dlpf_fs_sel_1 = 1<<4,
	
	//Gyro registers
	gyro_xout_h = 0x1D,
	gyro_xout_l = 0x1E,
	gyro_yout_h = 0x1F,
	gyro_yout_l = 0x20,
	gyro_zout_h = 0x21,
	gyro_zout_l = 0x22,
	
	temp_out_h = 0x1B,
	temp_out_l = 0x1C,
	
	who_am_i = 0x00,
	smplrt_div = 0x15,
	dlpf_fs = 0x16,
	int_cfg = 0x17,
	int_status = 0x1A,
	pwr_mgm = 0x3E,
	
};

typedef struct {
	int x;
	int y;
	int z;
} Data;

void setup() 
{
	unsigned char data;

	Serial.begin(9600);
	Wire.begin();
	
	//GYRO
	//+/- 2000 degrees per sec. Internal sample rate 1kHz, low pass filter 188Hz
	i2cWrite(gyro_dev, dlpf_fs, (dlpf_fs_sel_0 | dlpf_fs_sel_1 | dlpf_cfg_0));
	//sample rate divider 1kHz/10 = 100Hz
	i2cWrite(gyro_dev, smplrt_div, 9);
}

void loop()
{
	//Gyro supports interrupt to see when new data is available -- might be good to use
	p(gyro('x')); p('\t'); p(gyro('y')); p('\t'); pln(gyro('z'));
	delay(10);

}

int gyro(char orient)
{
	int d = 0;
	char gyro_reg;
	
	//Choose register
	switch(orient) {
		case 'x':
			gyro_reg = gyro_xout_h;
			break;
		case 'y':
			gyro_reg = gyro_yout_h;
			break;
		case 'z':
			gyro_reg = gyro_zout_h;
			break;
		default:
			pln("ERROR USE OF gyro()");
			exit(3);
	}

	Wire.beginTransmission(gyro_dev);
	Wire.write(gyro_reg);
	Wire.endTransmission(gyro_dev);
	Wire.requestFrom(gyro_dev, 2);
	
	if(Wire.available()) 
	{
		d = (Wire.read()<<8);
		
		if(Wire.available()) 
		{
			d |= Wire.read();
		} else {
			d = 0;
			pln("ERROR READING GYRO");
			exit(3);
		}
	} else {
		d = 0;
		pln("ERROR READING GYRO");
		exit(3);
	}
	Wire.endTransmission();
	return d;
}
	
void i2cWrite(char device, char reg, char data)
{
	Wire.beginTransmission(device);
	Wire.write(reg); 	//The register we wanna write to
	Wire.write(data);
	Wire.endTransmission();
}
	
unsigned char i2cRead(char device, char reg)
{
	unsigned char data;

	//We say what register we wanna read from.
	Wire.beginTransmission(device);
	Wire.write(reg);
	Wire.endTransmission(device);
	
	Wire.requestFrom(device, 1);
	if(Wire.available()) {
		data = Wire.read();
	} else {
		data = 0xEF;
		pln("Error reading "); p(device); p(" "); p(reg);
	}
	Wire.endTransmission();
	
	return data;
}

	