
############################################
##     Hand Gesture Recognition Client    ##
############################################
## Author: Michal Balogh, xbalog06        ##
## Date: 14.10.2024                       ##
############################################

import os
import sys
import cv2
import numpy as np
import torch
import mediapipe as mp
import socket
import time
from gesture_recognition_model import GestureRecognitionModel

# Ignore some deprecation warnings from protobuf lib
import warnings
warnings.filterwarnings("ignore", category=UserWarning,
                        module="google.protobuf")
warnings.filterwarnings("ignore", category=FutureWarning, module="torch")

############################################
##               Constants                ##
############################################
TIME_BETWEEN_GESTURES = 3
SERVER_IP = '192.168.0.48'
SERVER_PORT = 80
CAMERA_INDEX = 0
MIN_DETECTION_CONFIDENCE = 0.3
MIN_TRACKING_CONFIDENCE = 0.3

# Mapping of the gesture labels to the actual gestures
LABELS_MAPPING = {
    0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E',
    5: 'F', 6: 'G', 7: 'H', 8: 'I', 9: 'J',
    10: 'K', 11: 'L', 12: 'M', 13: 'N', 14: 'O',
    15: 'P', 16: 'Q', 17: 'R', 18: 'S', 19: 'T',
    20: 'U', 21: 'V', 22: 'W', 23: 'X', 24: 'Y',
    25: 'Z', 26: 'del', 27: 'space'
}


def get_hand_landmarks(image, hands):
    """
    Extracts hand landmarks from the given image using MediaPipe Hands module.

    Args:
        image: The input image from which landmarks are to be detected.
        hands: The MediaPipe Hands instance for detecting landmarks.

    Returns:
        tuple: Tuple where the first element is the input image with drawn landmarks,
               and the second element is a list of 1D arrays representing hand landmarks
               or None if no hand was detected.
    """
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    # Process the image and get the hand landmarks
    results = hands.process(image_rgb)

    if not results.multi_hand_landmarks:
        return image, None

    landmarks_list = []
    for hand_landmarks in results.multi_hand_landmarks:
        # Draw landmarks on the image
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style()
        )
        landmarks = np.array(
            [[lm.x, lm.y, lm.z] for lm in hand_landmarks.landmark]
        ).flatten()  # Flatten to 1D array

        landmarks_list.append(landmarks)

    return image, landmarks_list


def predict_gesture(landmarks):
    """
    Predicts letter based on hand landmarks using the pre-trained gesture recognition model.

    Args:
        landmarks: A list or array of hand landmarks representing a hand pose.

    Returns:
        str: The predicted gesture as a class label ("A", "B", "C", ...), or "Unknown" if the gesture could not be mapped to the labels.
    """
    # Add batch dimension
    landmarks_tensor = torch.tensor(
        landmarks, dtype=torch.float32).to(device).unsqueeze(0)
    with torch.no_grad():
        output = model(landmarks_tensor)
        _, predicted = output.max(1)

    predicted_index = predicted.item()
    predicted_label = LABELS_MAPPING.get(predicted_index, "Unknown")

    return predicted_label


def send_gesture_to_server(gesture, client_socket):
    """
    Sends the predicted gesture to the server via the provided socket.

    Args:
        gesture: The gesture to be sent to the server.
        client_socket: The socket used to communicate with the server.
    """
    try:
        client_socket.sendall(gesture.encode())
        print(f"Sent gesture: {gesture}", end='')
    except socket.error as e:
        print(f"Error sending gesture: {e}", file=sys.stderr)


def send_cooldown_passed(current_time, last_sent_time):
    """
    Checks wheter enough time passed since the last gesture was sent.

    Args:
        current_time: The current time in seconds since the epoch.
        last_sent_time: The time the last gesture was sent.

    Returns:
        bool: True if enough time has passed, False otherwise.
    """
    return current_time - last_sent_time >= TIME_BETWEEN_GESTURES


def display_gesture_on_frame(frame, gesture):
    """
    Displays the predicted gesture as text on the camera frame for user feedback.

    Args:
        frame: The camera frame where the gesture text should be displayed.
        gesture: The gesture to be displayed on the frame.
    """
    cv2.putText(img=frame,
                text=f'Gesture: {gesture}',
                org=(10, 30),
                fontFace=cv2.FONT_HERSHEY_SIMPLEX,
                fontScale=1,
                color=(255, 0, 0),
                thickness=2,
                lineType=cv2.LINE_AA)


def start_sending_gestures(client_socket):
    """
    Captures hand gestures from the camera, predicts their coresponding letter, and sends the letter to the server at specified intervals.

    Args:
        client_socket: The socket used to communicate with the server.
    """
    # Initialize the MediaPipe Hands module
    hands = mp_hands.Hands(static_image_mode=False,
                           min_detection_confidence=MIN_DETECTION_CONFIDENCE, min_tracking_confidence=MIN_TRACKING_CONFIDENCE)

    # Open the camera
    cap = cv2.VideoCapture(CAMERA_INDEX)
    if not cap.isOpened():
        print("Error: Could not open camera.", file=sys.stderr)
        return

    last_sent_time = time.time()  # Initialize the last sent time

    # Start reading the camera feed and sending the gestures
    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Could not read frame.", file=sys.stderr)
            break

        frame_with_landmarks, landmarks = get_hand_landmarks(frame, hands)

        if landmarks is not None:
            for landmark in landmarks:
                gesture = predict_gesture(landmark)
                current_time = time.time()  # Get the current time

                # Check if 5 seconds have passed since the last sent time
                if send_cooldown_passed(current_time, last_sent_time):
                    send_gesture_to_server(gesture + '\n', client_socket)
                    last_sent_time = current_time  # Update the last sent time

                display_gesture_on_frame(frame_with_landmarks, gesture)

        cv2.imshow('Hand Landmarks', frame_with_landmarks)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()


def run_client(server_ip=SERVER_IP, server_port=SERVER_PORT):
    """
    Starts the client that connects to a server and sends hand gesture data. Main entry point of the program.

    Args:
        server_ip: IP adress of the server. Defaults to SERVER_IP.
        server_port: Port of the server. Defaults to SERVER_PORT.
    """
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Attempt to connect to the ESP8266 server
    try:
        print(f"Connecting to server {server_ip}:{server_port}...")
        client_socket.connect((server_ip, server_port))
        print(f"Connected!")
    except socket.error as e:
        print(f"Error connecting to server: {e}", file=sys.stderr)
        return

    start_sending_gestures(client_socket)

    client_socket.close()
    print("Client disconnected")


if __name__ == '__main__':
    # Initialize MediaPipe Hands for getting hand landmarks
    mp_hands = mp.solutions.hands
    mp_drawing = mp.solutions.drawing_utils
    mp_drawing_styles = mp.solutions.drawing_styles

    # Load the trained model
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    input_size = 63  # 21 landmarks * 3 coordinates (x, y, z)
    num_classes = 28  # 26 alphabets + 2 special characters
    model = GestureRecognitionModel(
        input_size=input_size, num_classes=num_classes).to(device)
    model.load_state_dict(torch.load(
        "gesture_recognition_model.pth", weights_only=True))
    model.eval()  # Set the model for inference

run_client()
