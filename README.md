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
