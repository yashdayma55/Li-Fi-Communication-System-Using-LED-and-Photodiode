# Reliability Tests

These tests focus on ACK reliability and retransmission behavior
described in the report.

## ACK Flow
Sender -> DATA
Receiver -> ACK
Timeout -> Retransmit

## Observations from Report
- ACK logic enabled reliable message delivery at very short distances.
- Reliability drops sharply beyond 0.2–0.3 cm due to weak optical signal.
- Ambient light and alignment sensitivity were the primary failure sources.

## Known Challenges
- LED intensity falls off quickly with distance.
- Photodiode alignment is highly directional.
- Fixed thresholds are vulnerable to ambient light changes.
