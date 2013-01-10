#include <SD.h>
#include <Wire.h>

/*

A5 - SCL
A4 - SDA

*/

#define pln(x) Serial.println(x)
#define p(x) Serial.print(x)

enum {
	//Gyro Stuff
	gyro_dev = (0xD0>>1), //Removing the r/w bit

	dlpf_cfg_0 = 1<<0,
	dlpf_cfg_1 = 1<<1,
	dlpf_cfg_2 = 1<<2,
	dlpf_fs_sel_0 = 1<<3,
	dlpf_fs_sel_1 = 1<<4,
	
	
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
	
	//Accelerometer Stuff
	acc_dev = (0xA6>>1), //Removing  the r/w bit
	
	power_ctl = 0x2D,
	data_format = 0x31,
	
	datax0 = 0x32,
	datax1 = 0x33,
	datay0 = 0x34,
	datay1 = 0x35,
	dataz0 = 0x36,
	dataz1 = 0x37,
	
	acc_measure = 1<<3,
	acc_full_res = 1<<3,
	acc_range_2g = 0,
	acc_range_4g = 1,
	acc_range_8g = 1<<1,
	acc_range_16g = (1<<0 | 1<<1),
	
	
};

struct Data {
	int x;
	int y;
	int z;
};

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
	
	//ACC
	i2cWrite(acc_dev, data_format, acc_range_4g);
	i2cWrite(acc_dev, power_ctl, (acc_measure));		//Tell the acc to start measuring
}

void loop()
{
	struct Data gyro, acc;

	gyro = gyroData();
	acc = accData();
	p(gyro.x); p('\t'); p(gyro.y); p('\t'); p(gyro.z);
	p("|| "); 
	p(acc.x); p('\t'); p(acc.y); p('\t'); pln(acc.z);
	delay(10);

}
struct Data accData(void)
{
	int *g;
	struct Data d;
	
	g = i2cReadBytes(acc_dev, datax0, 6);
	
	d.x = ((g[1]<<8) | g[0]);
	d.y = ((g[3]<<8) | g[2]);
	d.z = ((g[5]<<8) | g[4]);
	
	free(g);
	return d;
}

struct Data gyroData(void)
{
	int *x, *y, *z;
	struct Data d;
	
	x = i2cReadBytes(gyro_dev, gyro_xout_h, 2);
	y = i2cReadBytes(gyro_dev, gyro_yout_h, 2);
	z = i2cReadBytes(gyro_dev, gyro_zout_h, 2);
	
	d.x = ((x[0]<<8) | x[1]);
	d.y = ((y[0]<<8) | y[1]);
	d.z = ((z[0]<<8) | z[1]);
	
	free(x);free(y);free(z);
	return d;
}

int* i2cReadBytes(char device, char reg, int nrBytes)
{
	int i, *d;
	
	d = (int*) malloc(nrBytes);

	Wire.beginTransmission(device);
	Wire.write(reg);
	Wire.endTransmission(device);
	Wire.requestFrom(device, nrBytes);
	
	for(i=0; i<nrBytes; i++) {
		if(Wire.available()) 
		{
			d[i] = Wire.read();
		} else {
			pln("ERROR READING i2cReadBytes");
			exit(3);
		}
	}
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

	