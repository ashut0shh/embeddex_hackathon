# embeddex_hackathon


Non-Invasive NIR Glucose Monitoring and Foot-Ulcer Risk Assessment System

This project presents a low-cost biomedical sensing system designed to estimate blood-glucose trends non-invasively using Near-Infrared (NIR) photoplethysmography, while simultaneously monitoring foot-temperature asymmetry to assess ulcer-formation risk in diabetic patients. The system integrates dual Arduino Unos, MATLAB signal-processing, VS Code–based data logging, and a Bluetooth telemetry interface for mobile monitoring.

1. NIR Sensor Acquisition on Arduino Uno

A custom analog front-end was built using an NIR LED–photodiode pair tuned to glucose-sensitive wavelengths. The primary Arduino Uno sampled the reflected NIR intensity using its ADC, applying oversampling and simple noise filtering. This board streamed data continuously over serial USB to a custom VS Code logging interface.

VS Code Data Logging

A lightweight Python-based serial logger running inside VS Code captured time-stamped NIR signals at fixed sampling intervals. This created an easily exportable dataset used for offline and real-time MATLAB processing. The workflow enabled rapid debugging, reproducibility, and clean integration with MATLAB scripts.

2. MATLAB Processing and Glucose Trend Extraction

MATLAB was used as the main interpretation layer. The NIR waveform was denoised (Butterworth filtering + moving average), normalized, and analyzed for intensity-based absorption changes correlated with glucose variations. Statistical calibration models such as multivariate regression or PLS (Partial Least Squares) were used to infer glucose trends. MATLAB scripts dynamically read the live serial log from VS Code, allowing near-real-time analysis, visualization, and comparison against manually recorded glucose values for validation.

This pipeline created a complete loop: acquire → log → filter → calibrate → trend detection.

3. Thermistor-Array Foot-Ulcer Monitoring on Second Arduino

A second Arduino Uno was dedicated to a multi-point thermistor array placed under the foot. It sampled localized skin temperature to detect:
Temperature asymmetry between regions
Persistent hot spots (early sign of inflammation)
Poor circulation leading to cold zones
These features are clinically relevant indicators for diabetic foot ulcer risk.

4. Bluetooth Telemetry for Mobile Monitoring

The thermistor-array Arduino transmitted processed temperature and alert flags wirelessly using an HC-05 Bluetooth module. The data streamed to a mobile terminal app, allowing:
Live temperature visualization
Early detection of abnormal temperature gradients
Combined analysis with NIR glucose-trend data to anticipate metabolic instability
Both systems together provide a holistic picture of diabetic health status: NIR absorption for glucose fall/rise and thermal mapping for ulcer prediction.

5. Unified Insight

By combining NIR spectroscopy with peripheral thermal monitoring, the system offers a non-invasive, affordable alternative to continuous glucose monitoring and foot-ulcer screening. The modular design—MATLAB analytics, dual-Arduino acquisition, and Bluetooth telemonitoring—makes it suitable for wearable integration and real-world diabetic care applications.
