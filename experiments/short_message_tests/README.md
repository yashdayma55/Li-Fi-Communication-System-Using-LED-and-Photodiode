# Short Message Tests

This folder captures short, framed message tests across OOK, PWM, and FSK.
All results below are summarized from the report.

## Test Setup
- Distance range: 0.1 cm, 0.2 cm, 0.3 cm, 0.4 cm
- Hardware: 2x Arduino Uno, 5 mm LEDs, photodiodes
- Receiver: photodiode + 10k ohm pull-up, analogRead()
- Protocol: preamble + length + payload + checksum + ACK

## OOK (Short Framed Message)
Frame format:
| HEADER (0x45) | LENGTH | PAYLOAD | CHECKSUM |

Results (approx. BER from report):
| Distance (cm) | Observed outcome | Approx. BER |
| --- | --- | --- |
| 0.1 | Stable decoding, clear threshold separation | 0.0 |
| 0.2 | Occasional packet loss, checksum mostly passes | 0.02 |
| 0.3 | Mixed performance, weak signal | 0.10 |
| 0.4 | Not reliable | N/A |

## PWM (Short Framed Message)
Results (approx. BER from report):
| Distance (cm) | Observed outcome | Approx. BER |
| --- | --- | --- |
| 0.1 | Stable decoding, clear pulse distinction | 0.10 |
| 0.2 | Timing drift, some frame failures | 0.30 |
| 0.3 | Highly unstable, noise-sensitive | 0.50 |
| 0.4 | Not reliable | N/A |

## FSK (Short Framed Message)
Results (approx. BER from report):
| Distance (cm) | Observed outcome | Approx. BER |
| --- | --- | --- |
| 0.1 | Inconsistent transition counting | 0.50 |
| 0.2 | Very weak performance | 0.80 |
| 0.3 | Almost no meaningful decoding | 0.95 |
| 0.4 | Not reliable | N/A |
