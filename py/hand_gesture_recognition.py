import cv2
import numpy as np
import torch
import mediapipe as mp
import time
import sys
from gesture_recognition_model import GestureRecognitionModel
from message_constructor import MessageConstructor
from constants import *


class HandGestureRecognition:

    def __init__(self, model_path, time_between_gestures=5):
        self.model_path = model_path
        self.time_between_gestures = time_between_gestures
        self.labels_mapping = {i: chr(65 + i) if i < 26 else ['del', 'space'][i - 26] for i in range(28)}
        self.mp_hands = mp.solutions.hands
        self.mp_drawing = mp.solutions.drawing_utils
        self.mp_drawing_styles = mp.solutions.drawing_styles

        self.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
        self.model = self.load_model()

        self.hands = self.mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.3, min_tracking_confidence=0.3)

    def load_model(self):
        input_size = 63  # 21 landmarks * 3 coordinates (x, y, z)
        num_classes = 28  # 26 alphabets + 2 special characters
        model = GestureRecognitionModel(input_size=input_size, num_classes=num_classes).to(self.device)
        model.load_state_dict(torch.load(self.model_path))
        model.eval()
        return model

    def get_hand_landmarks(self, image):
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
        results = self.hands.process(image_rgb)

        if results.multi_hand_landmarks:
            landmarks_list = []
            for hand_landmarks in results.multi_hand_landmarks:
                self.mp_drawing.draw_landmarks(
                    image,
                    hand_landmarks,
                    self.mp_hands.HAND_CONNECTIONS,
                    self.mp_drawing_styles.get_default_hand_landmarks_style(),
                    self.mp_drawing_styles.get_default_hand_connections_style()
                )
                landmarks = np.array(
                    [[lm.x, lm.y, lm.z] for lm in hand_landmarks.landmark]
                ).flatten()
                landmarks_list.append(landmarks)

            return image, landmarks_list
        return image, None

    def predict_gesture(self, landmarks):
        landmarks_tensor = torch.tensor(landmarks, dtype=torch.float32).to(self.device).unsqueeze(0)  # Add batch dimension
        with torch.no_grad():
            output = self.model(landmarks_tensor)
            _, predicted = output.max(1)

        predicted_index = predicted.item()
        return self.labels_mapping.get(predicted_index, "Unknown")

    def start(self, serial_communication, adress, command):
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("Error: Could not open camera.")
            return

        last_sent_time = time.time()

        message = MessageConstructor.init_message(address=adress, command=command)
        pos = 0
        line = 0
        while True:
            ret, frame = cap.read()
            if not ret:
                print("Error: Could not read frame.")
                break

            frame_with_landmarks, landmarks = self.get_hand_landmarks(frame)

            if landmarks is not None:
                for landmark in landmarks:
                    symbol = self.predict_gesture(landmark)
                    current_time = time.time()

                    # Send only every 5 seconds
                    if current_time - last_sent_time >= self.time_between_gestures:
                        print(f"Sending gesture: {symbol}")
                        MessageConstructor.set_symbol(message, symbol, pos, line)
                        print(message)
                        pos += 1 % SYMBOLS_PER_LINE
                        if pos == SYMBOLS_PER_LINE:
                            line += 1 % NUM_LINES

                        if serial_communication.send(message) != len(message):
                            print("Error sending gesture to client", file=sys.stderr)

                        last_sent_time = current_time  # Update the last sent time

                    cv2.putText(frame_with_landmarks, f'Gesture: {symbol}', (10, 30),
                                cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

            cv2.imshow('Hand Landmarks', frame_with_landmarks)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

        cap.release()
        cv2.destroyAllWindows()
