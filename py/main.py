from hand_gesture_recognition import HandGestureRecognition
from serial_communication import SerialCommunication
from message_constructor import MessageConstructor

MODEL_PATH = "gesture_recognition_model.pth"
SERIAL_PORT = "/dev/tty0"
TIME_BETWEEN_GESTURES = 5  # in seconds



if __name__ == "__main__":
    hand_gesture_recognition = HandGestureRecognition(model_path=MODEL_PATH,
                                                      time_between_gestures=TIME_BETWEEN_GESTURES)

    serial_communication = SerialCommunication(SERIAL_PORT)

    address = 0xFF
    command = 0x83
    hand_gesture_recognition.start(serial_communication=serial_communication, adress=address, command=command)

    serial_communication.close()