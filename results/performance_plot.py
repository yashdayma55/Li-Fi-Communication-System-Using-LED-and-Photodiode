import csv
from pathlib import Path

import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parent
CSV_PATH = BASE_DIR / "bit_error_rate.csv"
OUTPUT_PATH = BASE_DIR.parent / "docs" / "ber_comparison.png"


def load_series():
    distances = []
    ook = []
    fsk = []
    pwm = []

    with CSV_PATH.open(newline="", encoding="utf-8") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            distance = float(row["distance_cm"])
            if not row["ook"] or not row["fsk"] or not row["pwm"]:
                continue
            distances.append(distance)
            ook.append(float(row["ook"]))
            fsk.append(float(row["fsk"]))
            pwm.append(float(row["pwm"]))

    return distances, ook, fsk, pwm


def plot():
    distances, ook, fsk, pwm = load_series()

    plt.figure(figsize=(7.5, 4.5))
    plt.plot(distances, ook, marker="o", label="OOK")
    plt.plot(distances, fsk, marker="o", label="FSK")
    plt.plot(distances, pwm, marker="o", label="PWM")

    plt.xlabel("Distance (cm)")
    plt.ylabel("Bit Error Rate")
    plt.title("BER vs Distance for LiFi Modulation Techniques")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()

    OUTPUT_PATH.parent.mkdir(parents=True, exist_ok=True)
    plt.savefig(OUTPUT_PATH, dpi=180)
    print(f"Saved plot to {OUTPUT_PATH}")


if __name__ == "__main__":
    plot()
