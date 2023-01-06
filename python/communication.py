import serial
from settings import SERIAL_PORT, BAUDRATE

class CommunicationSettings:
    def __init__(self):
        self._port = SERIAL_PORT
        self._baudrate = BAUDRATE
        self._connection = None
        self._serial_read = ""

    def connect(self):
        try:
            self._connection = serial.Serial(
                port=self._port,
                baudrate=self._baudrate
            )
            self._connection.open()
        except:
            print("ERROR TO CONNECT SERIAL...")

    def read_serial(self):
        self._serial_read = self._connection.read(4)

    def get_params(self):
        return { "port": self._port, "baudrate": self._baudrate }

    def get_connector(self):
        return self._connection

    def get_confirmation(self):
        return Protocol.check_confirmation(self._serial_read)

    def get_end_of_thread(self):
        return Protocol.check_end_of_thread(self._serial_read)

    def send_protocol(self, protocol):
        self._connection.write(protocol.encode())

class Protocol:
    def __init__(self, connection):
        self._connection = connection
        self._thread = "F"
        self._protocol = "<T{}>".format(self._thread)

    def get_protocol(self):
        return self._protocol

    def turn_on_thread(self, direction):
        if direction == 'L':
            self._protocol = "<TL>"
        elif direction == 'R': 
            self._protocol = "<TR>"
            
        return self._protocol

    def turn_off_thread(self):
        self._protocol = "<TF>"
        return "<TF>"

    def get_strawberry(self):
        self._protocol = "<TG>"
        return "<TG>"

    @staticmethod
    def check_confirmation(response):
        if response == "<TS>".encode():
            return True
        return False

    @staticmethod
    def check_end_of_thread(response):
        if response == "<TE>".encode():
            return True
        return False