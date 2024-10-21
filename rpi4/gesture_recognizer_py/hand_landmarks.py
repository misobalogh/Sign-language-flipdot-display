import mediapipe as mp
import cv2
import numpy as np

# inspired from https://github.com/computervisioneng/sign-language-detector-python/blob/master/create_dataset.py

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles

hands = mp_hands.Hands(static_image_mode=True, min_detection_confidence=0.3)

image = cv2.imread('demo_img/hand.jpg')
image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

results = hands.process(image_rgb)

if results.multi_hand_landmarks:
    for hand_landmarks in results.multi_hand_landmarks:
        mp_drawing.draw_landmarks(
            image,
            hand_landmarks,
            mp_hands.HAND_CONNECTIONS,
            mp_drawing_styles.get_default_hand_landmarks_style(),
            mp_drawing_styles.get_default_hand_connections_style()
        )
        landmarks = np.array(
            [[lm.x, lm.y, lm.z] for lm in hand_landmarks.landmark]
        )
        print(landmarks)

cv2.imshow('Hand Landmarks', image)
cv2.waitKey(0)
cv2.destroyAllWindows()
