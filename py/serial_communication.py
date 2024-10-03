import serial

class SerialCommunication:
    def __init__(self, port_name, baudrate=57600):
        self.serial_port = self.open(port_name, baudrate)

    def open(self, port_name, baudrate):
        try:
            serial_port = serial.Serial(port_name, baudrate=baudrate, timeout=1)
            return serial_port
        except serial.SerialException as e:
            print(f"Error opening serial port: {e}")
            return None

    def close(self):
        if self.serial_port:
            self.serial_port.close()

    def send(self, message):
        if self.serial_port:
            bytes_sent = self.serial_port.write(message)
            if bytes_sent != len(message):
                print("Error writing to serial port")
            return bytes_sent
