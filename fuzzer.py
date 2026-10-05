import os
import shutil
import struct

ORIGINAL = "flight_data.bin"

if not os.path.exists(ORIGINAL):
    print("flight_data.bin not found.")
    print("Run the C program and save data first.")
    exit()

# 1. Truncated file
with open(ORIGINAL, "rb") as file:
    data = file.read()

with open("corrupt_truncated.bin", "wb") as file:
    file.write(data[:len(data) // 2])

# 2. Oversized file
with open("corrupt_oversized.bin", "wb") as file:
    file.write(data)
    file.write(b"EXTRA GARBAGE DATA")

# 3. Garbage binary data
garbage = bytearray(data)

for i in range(20, min(40, len(garbage))):
    garbage[i] = 255

with open("corrupt_garbage.bin", "wb") as file:
    file.write(garbage)

# 4. Invalid seat number
bad_seat = bytearray(data)

# First 4 bytes contain the first seat ID
bad_seat[0:4] = struct.pack("i", 999)

with open("corrupt_seat.bin", "wb") as file:
    file.write(bad_seat)

print("Corrupted test files created!")
print("Created:")
print("corrupt_truncated.bin")
print("corrupt_oversized.bin")
print("corrupt_garbage.bin")
print("corrupt_seat.bin")