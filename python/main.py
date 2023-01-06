import time
from communication import CommunicationSettings, Protocol
from camera import Camera
from services import RoboflowAPI
from settings import DELAY_SETUP, DELAY_GETTING, LATENCY, PICKER_TIME_INTERVAL, INTERVAL

webcam = Camera()
webcam.start()

serial = CommunicationSettings()
serial.connect()
connection = serial.get_connector()
protocol = Protocol(connection)

image_processor = RoboflowAPI()

while not connection.isOpen():
    print("STARTING CONNECTION AGAIN")
    serial.connect()
    
print("SERIAL CONNECTED")

while True:
    try:
        print("TAKING PICTURE")
        webcam.capture()

        print("IMAGE PROCESSING")
        image_processor.get_image_predicted()
        image_processor.get_json_predicted()

        motor_activity_time = image_processor.get_motor_activity_time()
        position_time = image_processor.get_motor_activity_time() if motor_activity_time != -1 else PICKER_TIME_INTERVAL
        action = protocol.get_strawberry() if motor_activity_time != -1 else protocol.turn_off_thread()
        position_changed = True if position_time != PICKER_TIME_INTERVAL else False

        if position_changed:
            if position_time < 0:
                print("THREAD GO LEFT")
                serial.send_protocol(protocol.turn_on_thread('L'))
            elif position_time > 0:
                print("THREAD GO LEFT")
                serial.send_protocol(protocol.turn_on_thread('R'))
        else:
            print("THREAD GO AWAY")
            serial.send_protocol(protocol.turn_on_thread('L'))

        time.sleep(abs(position_time))

        serial.send_protocol(action)
        if action == "<TG>":
            print("GETTING STRAWBERRY")
            time.sleep(DELAY_GETTING + LATENCY)

        serial.read_serial()
        if serial.get_end_of_thread():
            print("END OF THREAD")
            serial.send_protocol(protocol.turn_on_thread('R'))
            time.sleep(DELAY_SETUP)
            break

    except Exception as e:
        print("AN EXCEPTION OCCURRED")
        print(repr(e))
        time.sleep(INTERVAL * 3)
        continue

print("----- STOPPED -----")