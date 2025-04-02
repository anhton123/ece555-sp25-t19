# Summary
This repo contains all code created by Sp'25 ECE455/555 T19.

## System Info
To obtain the system information of a device, run `./src/system-info/system-info.sh`.

Example output:
```bash
$ ./src/system-info/system-info.sh 
Architecture: aarch64
Processor: Cortex-A76
Number of Processors: 4
Ubuntu Version: 24.10
Kernel Version: 6.11.0-1004-raspi
RAM Capacity: 7.8Gi
```

## Inference
To run the inference benchmark:
1. Plug a USB camera into your device
2. Confirm the USB camera exists by running:
```bash
$ ls /dev | grep video0
video0
```
3. `cd src/inference/`
4. Download yolo weights from [ultralytic](https://docs.ultralytics.com/models/yolov8/) and place them into the `yolo-weights` folder
5. Run: `python3 inference.py`

Your camera should now turn on and perform inferencing. You should see inference results similar to this:
```bash
0: 480x640 1 person, 96.6ms
Confidence ---> 0.93
Class name --> person
Speed: 6.3ms preprocess, 96.6ms inference, 6.4ms postprocess per image at shape (1, 3, 480, 640)
```

## Accelerometer
This code provides 3 user-space apps:
- accelerometer: app that prints out accelerometer data
- cpu_accelerometer: app that calculates magnitude of accelerometer data using the CPU
- gpu_accelerometer: app that calcualtes magnitude of accelerometer data using the GPU

To ensure either of these apps to work:
1. Plug in the ADXL345 to your devices i2c bus
2. Navigate to `src/accelerometer`
3. Replace all accelerometer code with the i2c bus your accelerometer is connected to

To run the `accelerometer` app:
1. `cd src/accelerometer`
2. `make accel`
3. `./accelerometer`

Example output of the `accelerometer` app:
```bash
$ ./accelerometer 
-----------------------------
Iteration 0
X-Axis Acceleration: -4.9627
Y-Axis Acceleration: -8.3481
Z-Axis Acceleration: -2.7314
-----------------------------
```

For both the `cpu_accelerometer` and `gpu_accelerometer` source code, you can replace the `DEBUG` flag from `0` to `1` to see debug prints.
 
To run the `cpu_accelerometer` app:
1. `cd src/accelerometer`
2. `make cpu`
3. `./cpu_accelerometer`

Example output of the `acceleroemter` app with `DEBUG` kept at 0:
```bash
$ ./cpu_accelerometer 
Collecting 1000 accelerometer samples...
CPU execution time: 0.000679 seconds
Program time: 0.000679 seconds
```


To run the `gpu_accelerometer` app:
1. `cd src/accelerometer`
2. `make gpu`
3. `./gpu_accelerometer`

Example output of the `gpu_acceleroemter` app with `DEBUG` changed to 1:
```bash
Collecting 1000 accelerometer samples...
-----------------------------
Iteration 0
X-Axis Acceleration: -4.9627
Y-Axis Acceleration: -8.3096
Z-Axis Acceleration: -2.7699
-----------------------------
...      <-- accelerometer data
GPU execution time: 0.000143 seconds
Program time: 0.093437 seconds
10.067320
...      <-- magnitude data
```
