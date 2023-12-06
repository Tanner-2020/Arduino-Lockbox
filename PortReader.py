import serial
ser = serial.Serial('COM3')
ser.flushInput()

while True:
        ser_bytes = ser.readline()
        decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        print(decoded_bytes)
        if decoded_bytes == "Status":
            ser.write(bytes(1, 'utf-8'))
            print("Success")
