Haptic Interface Array: High-Fidelity Gesture Control.


This repository contains the firmware and hardware schematics for a Biometric Gesture Control Glove. The system utilizes an ESP32-based architecture to translate organic human hand movements into precise mechanical actuation with minimal latency.

🚀 Key Features

Adaptive Calibration: Automatically establishes a sensor baseline at startup to account for environmental variables and different hand dimension.

Hysteretic Logic Algorithm: Implements a dual-threshold "Dead-Zone" (9-unit trigger / 1-unit release) to eliminate signal oscillation and "jitter."

Digital Signal Smoothing: Uses a multi-sample moving average filter to ensure fluid motion.

Low-Latency Transmission: Optimized Bluetooth Serial packet structure for near-instant response.

🛠 Hardware Components

Microcontroller: 2x ESP32 (Transmitter & Receiver)

Sensors: 5x 2.2" Flex Sensors (Resistance-based)

Actuators: 5x MG996R/SG90 High-Torque Servos

Resistors: 5x 10k$\Omega$ Voltage Dividers

Power: 7.4V Li-ion (Servo Rail) + 5V (ESP32)

📂 Project Structure

/Transmitter: Firmware for the Haptic Glove, featuring auto-calibration and signal processing.

/Receiver: Firmware for the Robotic Hand, handling packet parsing and servo actuation.

/Schematics: Detailed wiring diagrams for ADC1 pin mapping and common ground power rails.

⚙️ Operational Logic (The "9/1" Split)

To combat the electrical noise inherent in high-resistance flex sensors, this project utilizes a custom Hysteresis Loop:

Activation: The finger must bend past a 9-unit drop from the baseline to trigger an "Active" state.

Deactivation: The finger only returns to an "Inactive" state once it reaches within 1 units of the baseline equilibrium. 

🔧 Installation

Open /Transmitter/Transmitter.ino in Arduino IDE.

Update the armAddress[] with your Receiver's MAC address.

Update the units of the threshold to your needs

Flash both ESP32 boards and power the servo rail externally.

📈 Future Roadmap

Integration of myoware.
Migration to ADC1-exclusive pin mapping to eliminate Bluetooth/ADC2 interference.

Implementation of Non-Volatile Storage (NVS) to save calibration profiles.

PCB design for a wearable, compact form factor.

Author: [Nandhu Bharathan]

Connect: [www.linkedin.com/in/nandhu-bharathan-94b619290]
