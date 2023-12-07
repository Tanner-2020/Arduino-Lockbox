import serial
ser = serial.Serial('/dev/ttyACM0',9600)
ser.flushInput()
lastSent = ""
status_flag = False
ID_flag = False


start_file = open("resumedata.txt","r")
starting_vals = start_file.readline()
starting_vals = starting_vals[0:len(starting_vals)-1]
print(starting_vals)
array = starting_vals.split(" ",-1)
print(array)

while True:

    # TODO: Create file reader to load starting values from    
    

    # Establishes serial connection to Arduino
    ser_bytes = ser.readline()
    ser_bytes = str(ser_bytes)
    mod_ser_bytes = ser_bytes[2:len(ser_bytes)-5]
    print(mod_ser_bytes)

    # Code for arduino initialization
    if mod_ser_bytes == "Status":
        ser.write(bytes(array[1], 'utf-8'))
        status_flag = True
    elif mod_ser_bytes == "ID":
        ser.write(bytes(array[2], 'utf-8'))
        ID_flag = True

    # Code for file writing
    elif status_flag == True and ID_flag == True:
        file_object = open("logs.txt", "a")
        file_object.writelines(mod_ser_bytes + "\n")
        file_object.close()
        print(ser_bytes)
