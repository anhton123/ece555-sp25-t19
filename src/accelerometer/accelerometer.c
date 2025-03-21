// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADXL345
// This code is designed to work with the ADXL345_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Accelorometer?sku=ADXL345_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

typedef struct {
	int x_accl;
	int y_accl;
	int z_accl;
} accelerometer;

// initializes the adxl345 device
int adxl345_init() {
	int file;
	char *bus = "/dev/i2c-1";
	char config[2]={0};
	
	// get a fd on the I2C bus
	if ((file = open(bus, O_RDWR)) < 0) {
		printf("Failed to open the bus. \n");
		exit(1);
	}
	
	// establish role as I2C slave
	ioctl(file, I2C_SLAVE, 0x53);


	// Bandwidth Register (0x2C), 
	// Output Data rate = 100Hz (0x0A)
	config[0] = 0x2C;
	config[1] = 0x0A;
	write(file, config, 2);
	
	// Select Power control register(0x2D)
	// Auto-sleep disable(0x08)
	config[0] = 0x2D;
	config[1] = 0x08;
	write(file, config, 2);
	
	// Select Data format register(0x31)
	// Self test disabled, 4-wire interface, Full resolution, range = +/-2g(0x08)
	config[0] = 0x31;
	config[1] = 0x08;
	write(file, config, 2);
	
	sleep(1);
	return file;
}

// reads the accelerometer data
void adxl345_get_accel(const int fd, accelerometer *a) {
	// Read 6 bytes of data from register(0x32)
	// xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
	char reg[1] = {0x32};
	write(fd, reg, 1);
	char data[6] ={0};
	if(read(fd, data, 6) != 6)
	{
		printf("Erorr : Input/output Erorr \n");
		exit(1);
	}
	else
	{
		// Convert the data to 10-bits
		a->x_accl = ((data[1] & 0x03) * 256 + (data[0] & 0xFF));
		if(a->x_accl > 511) a->x_accl -= 1024;

		a->y_accl = ((data[3] & 0x03) * 256 + (data[2] & 0xFF));
		if(a->y_accl > 511) a->y_accl -= 1024;

		a->z_accl = ((data[5] & 0x03) * 256 + (data[4] & 0xFF));
		if(a->z_accl > 511) a->z_accl -= 1024;
	}
}

int main() 
{
	int fd, sleep_duration_us, num_samples;
	accelerometer *accl;
	
	
	sleep_duration_us = 1000*500; // 500ms sleep
	num_samples = 100;
	accl = (accelerometer *)malloc(sizeof(accelerometer));
	if (accl == NULL){
		printf("Could not create accl pointer\n");
		exit(1);
	}
	
	fd = adxl345_init();
	for (int i=0; i<num_samples; i++) {
		adxl345_get_accel(fd, accl);
		printf("-----------------------------\n");
		printf("Iteration %d\n", i);
		printf("X-Axis Acceleration: %d \n", accl->x_accl);
		printf("Y-Axis Acceleration: %d \n", accl->y_accl);
		printf("Z-Axis Acceleration: %d \n", accl->z_accl);
		printf("-----------------------------\n");
		usleep(sleep_duration_us);
	}

	free(accl);
	return 0;
}
