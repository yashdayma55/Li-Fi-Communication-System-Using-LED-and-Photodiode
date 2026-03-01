# Performance Analysis

Summary aligned with the report.

## Environment
- Ambient light: indoor lab environment with cardboard enclosure to reduce noise.
- Distance range: 0.1 cm to 0.4 cm.
- Hardware: 2x Arduino Uno, 5 mm LEDs, photodiodes, 220 ohm LED resistor, 10k ohm pull-up.
- Channel: direct LED-to-photodiode optical path, no analog amplification.

## Observations
- OOK: most reliable at 0.1–0.2 cm; instability grows at 0.3 cm; fails at 0.4 cm.
- PWM: moderate performance; more sensitive to timing drift; degrades quickly beyond 0.2 cm.
- FSK: weakest performance; transition counting fails at larger distances.
