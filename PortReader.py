import serial
ser = serial.Serial('/dev/ttyACM0')
import re
ser.flushInput()

while True:
        ser_bytes = ser.readline()
        decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
        print(decoded_bytes)
        ser_bytes = re.sub("[b,\r,\n]","",ser_bytes)
        outputFile = open("arduinolog.txt","a")
        outputFile.write(ser_bytes)
        outputFile.write("\n")
        outputFile.close
    except:
        print("Keyboard Interrupt")
        break


# testinput = 'b1 0 1044346547\r\n'


