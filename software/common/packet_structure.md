# Packet Structure and Timing

## Packet Format
| PREAMBLE | LENGTH | DATA | CHECKSUM |

Suggested fields (per report):
- PREAMBLE: fixed sync byte (0xA5 or 0xAA)
- LENGTH: number of payload bytes
- DATA: payload bytes
- CHECKSUM: 8-bit sum of payload bytes (mod 256)

## ACK Flow
Sender -> DATA
Receiver -> ACK
Timeout -> Retransmit

## Timing Notes
Document your actual values here once finalized:
- Bit duration (ms):
- Symbol duration (ms):
- Sampling rate (Hz):
- ACK timeout (ms):

## Modulation Mapping (example)
- OOK: LED on = 1, LED off = 0
- FSK: low frequency = 0, high frequency = 1
- PWM: low duty cycle = 0, high duty cycle = 1
