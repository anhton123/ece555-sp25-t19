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
#include <math.h>
#include <time.h>

#define DEBUG (0)  // replace with value '1' to see accel and mag prints

typedef struct {
	float x_accl;
	float y_accl;
	float z_accl;
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
void adxl345_get_accel(const int fd, accelerometer *accl, int idx) {
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
		// Convert accelerometer to be between -9.81 to 9.81
		accl[idx].x_accl = ((data[1] & 0x03) * 256 + (data[0] & 0xFF));
		if(accl[idx].x_accl > 511) accl[idx].x_accl -= 1024;
		accl[idx].x_accl = ((accl[idx].x_accl * 9.81)/ 255);

		accl[idx].y_accl = ((data[3] & 0x03) * 256 + (data[2] & 0xFF));
		if(accl[idx].y_accl > 511) accl[idx].y_accl -= 1024;
		accl[idx].y_accl = ((accl[idx].y_accl * 9.81)/ 255);

		accl[idx].z_accl = ((data[5] & 0x03) * 256 + (data[4] & 0xFF));
		if(accl[idx].z_accl > 511) accl[idx].z_accl -= 1024;
		accl[idx].z_accl = ((accl[idx].z_accl * 9.81) / 255);
	}
}

// CPU implementation to calculate magnitude
void cpu_compute_magnitude(accelerometer* accl, float *magnitude, int num_samples) {
	for (int i=0; i<num_samples; i++) {
		magnitude[i] = sqrtf(accl[i].x_accl*accl[i].x_accl + accl[i].y_accl*accl[i].y_accl + accl[i].z_accl*accl[i].z_accl);
	}
}

int main() 
{
	int fd, sleep_duration_us, num_samples;
	struct timespec program_start, program_end, execution_start, execution_end;
	double program_time, execution_time;
	float *magnitude;
	accelerometer *accl;

	sleep_duration_us = 1000*1; // 1ms sleep
	num_samples = 1000;
	magnitude = (float *)malloc(num_samples * sizeof(float));
	accl = (accelerometer *)malloc(num_samples * sizeof(accelerometer));
	if (accl == NULL){
		printf("Could not create accl pointer\n");
		exit(1);
	}

	printf("Collecting %d accelerometer samples...\n", num_samples);
	fd = adxl345_init();
	for (int idx=0; idx<num_samples; idx++) {
		adxl345_get_accel(fd, accl, idx);
#if DEBUG == 1 
		printf("-----------------------------\n");
		printf("Iteration %d\n", idx);
		printf("X-Axis Acceleration: %.4f\n", accl[idx].x_accl);
		printf("Y-Axis Acceleration: %.4f\n", accl[idx].y_accl);
		printf("Z-Axis Acceleration: %.4f\n", accl[idx].z_accl);
		printf("-----------------------------\n");
#endif
		usleep(sleep_duration_us);
	}

	// measure time to run program
	clock_gettime(CLOCK_MONOTONIC, &program_start);    
	clock_gettime(CLOCK_MONOTONIC, &execution_start);
	cpu_compute_magnitude(accl, magnitude, num_samples);
	clock_gettime(CLOCK_MONOTONIC, &execution_end);
	clock_gettime(CLOCK_MONOTONIC, &program_end);

	// print program times
	execution_time = (execution_end.tv_sec - execution_start.tv_sec) + (execution_end.tv_nsec - execution_start.tv_nsec) / 1e9;
	program_time = (program_end.tv_sec - program_start.tv_sec) + (program_end.tv_nsec - program_start.tv_nsec) / 1e9;
	printf("CPU execution time: %.6f seconds\n", execution_time);    // Print first 5 results
	printf("Program time: %.6f seconds\n", execution_time);    // Print first 5 results

	// print magnitude values
#if DEBUG == 1
	for (int i=0; i<num_samples; i++){
		printf("Magnitude [%d] = %.6f\n", i, magnitude[i]);
	}
#endif

	free(accl); free(magnitude);
	return 0;
}
