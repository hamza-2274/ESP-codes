import serial
import numpy as np
import matplotlib.pyplot as plt

port = 'COM3'  # or '/dev/ttyUSB0'
baud = 115200
samples = 60000

ser = serial.Serial(port, baud, timeout=1)
input_vals = []
output_vals = []

print("Listening...")

while len(input_vals) < samples:
    line = ser.readline().decode().strip()
    if line:
        try:
            _, u, y = map(int, line.split(','))
            input_vals.append(u)
            output_vals.append(y)
        except:
            pass  # skip corrupt lines

ser.close()
print("Data received!")

# FFT
u = np.array(input_vals)
y = np.array(output_vals)

U = np.fft.fft(u)
Y = np.fft.fft(y)
H = Y / U

freq = np.fft.fftfreq(len(H), d=0.001)  # d = 1ms = 0.001s

# Only plot up to Nyquist
half = len(freq)//2
plt.figure(figsize=(12, 5))
plt.subplot(1,2,1)
plt.plot(freq[:half], 20*np.log10(np.abs(H[:half])))
plt.title("Magnitude Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude (dB)")

plt.subplot(1,2,2)
plt.plot(freq[:half], np.angle(H[:half], deg=True))
plt.title("Phase Response")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Phase (degrees)")
plt.tight_layout()
plt.show()
