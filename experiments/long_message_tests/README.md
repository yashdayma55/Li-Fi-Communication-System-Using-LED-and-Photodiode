# Long Message Tests

This folder captures long text transmission using packetization or
streaming methods described in the report.

## OOK (Long Message, UART-style)
Method summary:
- Start bit + 8 data bits (LSB-first) + stop bit
- Fixed bit period (approx. 40 ms in report)
- No explicit length field; terminator indicates end

Notes:
- Works for longer streams but sensitive to threshold drift.
- Reliable only at very close distance (0.1–0.2 cm).

## FSK (Long Message)
Method summary:
- Preamble 0xAA + LENGTH + PAYLOAD
- Bit value encoded by pulse duration (short vs long)

Notes:
- Performance degrades quickly with distance.
- Transition counting weak under low signal strength.

## PWM (Long Message, Chunked)
Method summary:
- Long text split into chunks (approx. 30 chars)
- Each chunk sent as [PREAMBLE][LENGTH][CHUNK]
- Stop-and-wait ACK between chunks

Notes:
- More reliable than raw streaming but still limited by timing drift.
- Reliable only at very short distances.
