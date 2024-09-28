import cv2
import numpy as np
import torch
import mediapipe as mp
from gesture_recognition_model import GestureRecognitionModel

labels_mapping = {
    0: 'A', 1: 'B', 2: 'C', 3: 'D', 4: 'E',
    5: 'F', 6: 'G', 7: 'H', 8: 'I', 9: 'J',
    10: 'K', 11: 'L', 12: 'M', 13: 'N', 14: 'O',
    15: 'P', 16: 'Q', 17: 'R', 18: 'S', 19: 'T',
    20: 'U', 21: 'V', 22: 'W', 23: 'X', 24: 'Y',
    25: 'Z', 26: 'del', 27: 'space'
}

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
input_size = 63  # 21 landmarks * 3 coordinates (x, y, z)
num_classes = 28  # 26 alphabets + 2 special characters
model = GestureRecognitionModel(input_size=input_size, num_classes=num_classes).to(device)
model.load_state_dict(torch.load("gesture_recognition_model.pth"))
model.eval()

def get_hand_landmarks(image):
    image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    results = hands.process(image_rgb)

    if results.multi_hand_landmarks:
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
    return image, None

def predict_gesture(landmarks):
    landmarks_tensor = torch.tensor(landmarks, dtype=torch.float32).to(device).unsqueeze(0)  # Add batch dimension
    with torch.no_grad():
        output = model(landmarks_tensor)
        _, predicted = output.max(1)

    predicted_index = predicted.item()
    predicted_label = labels_mapping.get(predicted_index, "Unknown")

    return predicted_label

if __name__ == '__main__':
    hands = mp_hands.Hands(static_image_mode=False, min_detection_confidence=0.3, min_tracking_confidence=0.3)

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        exit()

    while True:
        ret, frame = cap.read()
        if not ret:
            print("Error: Could not read frame.")
            break

        frame_with_landmarks, landmarks = get_hand_landmarks(frame)

        if landmarks is not None:
            for landmark in landmarks:
                gesture = predict_gesture(landmark)
                cv2.putText(frame_with_landmarks, f'Gesture: {gesture}', (10, 30),
                            cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)

        cv2.imshow('Hand Landmarks', frame_with_landmarks)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
