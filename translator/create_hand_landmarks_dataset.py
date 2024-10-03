import mediapipe as mp
import cv2
import os
import numpy as np
import pandas as pd

import warnings
warnings.filterwarnings("ignore")

mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils

hands = mp_hands.Hands(static_image_mode=True, max_num_hands=1, min_detection_confidence=0.3)

asl_dataset_dir = 'dataset/asl_alphabet_train' # path to the ASL dataset
landmark_dataset_file = 'dataset/asl_landmarks_dataset.csv'

data = []

# Loop through each class (A, B, C, ...)
for label in os.listdir(asl_dataset_dir):
    label_dir = os.path.join(asl_dataset_dir, label)
    if not os.path.isdir(label_dir):
        continue

    # Loop through each image in the class folder
    for img_file in os.listdir(label_dir):
        img_path = os.path.join(label_dir, img_file)

        image = cv2.imread(img_path)
        if image is None:
            continue

        # Convert the image to RGB for MediaPipe
        image_rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

        # Process the image to extract hand landmarks
        results = hands.process(image_rgb)

        # If landmarks are detected, extract them
        if results.multi_hand_landmarks:
            for hand_landmarks in results.multi_hand_landmarks:
                # Flatten the landmark coordinates into a single list
                landmarks = []
                for lm in hand_landmarks.landmark:
                    landmarks.extend([lm.x, lm.y, lm.z])

                data.append([label] + landmarks)

columns = ['label'] + [f'x{i}' for i in range(21)] + [f'y{i}' for i in range(21)] + [f'z{i}' for i in range(21)]
df = pd.DataFrame(data, columns=columns)
df.to_csv(landmark_dataset_file, index=False)

print(f"Landmark dataset saved to {landmark_dataset_file}.")
