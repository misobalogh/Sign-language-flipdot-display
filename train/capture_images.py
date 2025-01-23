import cv2
import mediapipe as mp
import time
import os

# Initialize MediaPipe Hands
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=2,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)
mp_draw = mp.solutions.drawing_utils

# Initialize video capture
cap = cv2.VideoCapture(0)

# Create directory for saving images
output_dir = "captured_hands"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# Initialize time for image capture
prev_capture_time = time.time()

while True:
    success, img = cap.read()
    if not success:
        print("Failed to capture frame")
        break

    # Convert image to RGB for MediaPipe
    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)

    # Draw hand landmarks
    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(
                img,
                hand_landmarks,
                mp_hands.HAND_CONNECTIONS
            )

    # Capture image every second
    current_time = time.time()
    if current_time - prev_capture_time >= 1.0:
        timestamp = int(current_time)
        filename = os.path.join(output_dir, f"hand_{timestamp}.jpg")
        cv2.imwrite(filename, img)
        print(f"Captured: {filename}")
        prev_capture_time = current_time

    # Display the frame
    cv2.imshow("Hand Tracking", img)

    # Break loop on 'q' press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Clean up
cap.release()
cv2.destroyAllWindows()
