# Li-Fi Communication System Using LED and Photodiode
<p align="left">
  <img src="https://img.shields.io/badge/Platform-Arduino-blue?style=flat-square" alt="Platform: Arduino" />
  <img src="https://img.shields.io/badge/Modulation-FSK%20%7C%20OOK%20%7C%20PWM-green?style=flat-square" alt="Modulation: FSK | OOK | PWM" />
  <img src="https://img.shields.io/badge/Status-Completed-brightgreen?style=flat-square" alt="Status: Completed" />
</p>

Multi-Modulation Visible Light Communication with ACK-based Reliability

## Overview
This project implements an end-to-end LiFi (Light Fidelity) communication system
using visible light for short-range wireless data transmission. It compares three
modulation schemes:

- Frequency Shift Keying (FSK)
- On-Off Keying (OOK)
- Pulse Width Modulation (PWM)

The system includes packet structuring, ACK-based reliability, and timeout-based
retransmission to improve delivery over an optical channel.

## System Architecture
The system is layered as:

Message Input -> Encoding -> Modulation (FSK/OOK/PWM) -> LED Transmitter
-> Light Channel -> Photodiode Receiver -> Demodulation -> Decoding -> Output

Architecture diagram: `docs/architecture_diagram.png`
Protocol flow: `docs/protocol_flow.png`
Full report: `docs/Design and Evaluation of a Li-Fi Communication System Using LED and Photodiode_report.pdf`

## Hardware Setup
Components used (per report):

- 2x Arduino Uno
- 5 mm visible light LED
- Photodiode
- 220 ohm current-limiting resistor (LED)
- 10k ohm pull-up resistor (receiver)
- 5V power supply
- USB serial interface

Example wiring:

Transmitter:
- LED -> GPIO pin (e.g., D5) via 220 ohm resistor
- GND -> common ground

Receiver:
- Photodiode -> analog pin (e.g., A0)
- 10k ohm resistor -> pull-up (reverse-biased photodiode)
- GND -> common ground

Hardware photo: `docs/hardware_setup.jpg`
Oscilloscope capture: `docs/oscilloscope_waveform.png`

Note: Replace these two images with photos from your real hardware setup
and oscilloscope captures for final publication.

## Software Modules
The software is organized by modulation scheme:

- `software/fsk/`
  - `sender/` frequency switching logic
  - `receiver/` signal detection and frequency analysis
  - `ack_protocol/` packet splitting and ACK handling
- `software/ook/`
  - `sender/` LED on/off modulation
  - `receiver/` threshold decoding and sync
- `software/pwm/`
  - `sender/` duty cycle mapping
  - `receiver/` pulse width decoding

Shared protocol notes: `software/common/packet_structure.md`

## Communication Protocol
Packet layout:

| PREAMBLE | LENGTH | DATA | CHECKSUM |

ACK flow:

Sender -> DATA
Receiver -> ACK
Sender waits for ACK
Timeout -> Retransmit

Timing notes and tuning: `software/common/packet_structure.md`

## Experimental Results
Results and plots live in `results/`:

- `results/performance_analysis.md`
- `results/bit_error_rate.csv`
- `docs/ber_comparison.png`

## Demo
Add a demo video link here (YouTube unlisted recommended).

## Future Improvements
- Forward error correction
- Adaptive frequency switching
- Encryption layer for secure LiFi
- Higher data rate optimization
- Ambient light filtering

## How To Run
Transmitter:
1. Open Arduino IDE.
2. Upload the sender sketch for your modulation scheme.
3. Connect LED circuit.
4. Open Serial Monitor and send a message.

Receiver:
1. Upload the receiver sketch.
2. Connect photodiode circuit.
3. Open Serial Monitor to view decoded message.

## Contributors
- Yash Dayma (George Mason University)
- Chaitanya Chaudhari (George Mason University)

## License
MIT License. See `LICENSE`.
