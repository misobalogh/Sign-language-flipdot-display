import cv2
import mediapipe as mp
import time
import os

mp_hands = mp.solutions.hands
hands = mp_hands.Hands(
    static_image_mode=False,
    max_num_hands=2,
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5
)
mp_draw = mp.solutions.drawing_utils


cap = cv2.VideoCapture(0)

base_dir = "captured_hands"
if not os.path.exists(base_dir):
    os.makedirs(base_dir)

current_letter = None
image_counter = 0
prev_capture_time = time.time()

def create_letter_directory(letter):
    letter_dir = os.path.join(base_dir, letter)
    if not os.path.exists(letter_dir):
        os.makedirs(letter_dir)
    return letter_dir

def get_next_image_number(letter_dir):
    existing_files = os.listdir(letter_dir) if os.path.exists(letter_dir) else []
    numbers = [-1]
    for filename in existing_files:
        try:
            number = int(os.path.splitext(filename)[0])
            numbers.append(number)
        except ValueError:
            continue
    return max(numbers) + 1

pause = False
while True:
    key = cv2.waitKey(1) & 0xFF

    if key == ord(' '): # SPACE
        pause = not pause
        print("Paused" if pause else "Resumed")

    if key == 27:  # ESC key
        break

    success, img = cap.read()
    if not success:
        print("Failed to capture frame")
        break

    clean_img = img.copy()

    img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = hands.process(img_rgb)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_draw.draw_landmarks(
                img,
                hand_landmarks,
                mp_hands.HAND_CONNECTIONS
            )

    if current_letter:
        cv2.putText(img, f"Recording letter: {current_letter.upper()}", (10, 50),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

    if pause:
        cv2.putText(img, "PAUSED", (10, 100),
                    cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)

    cv2.imshow("Hand Tracking", img)

    if current_letter and time.time() - prev_capture_time >= 1.0 and not pause:
        letter_dir = os.path.join(base_dir, current_letter.upper())
        filename = os.path.join(letter_dir, f"{image_counter}.jpg")
        cv2.imwrite(filename, clean_img)
        print(f"Captured: {filename}")
        image_counter += 1
        prev_capture_time = time.time()

    if ord('a') <= key <= ord('z'):
        current_letter = chr(key)
        letter_dir = create_letter_directory(current_letter.upper())
        image_counter = get_next_image_number(letter_dir)
        print(f"Now recording for letter: {current_letter.upper()}, starting at index {image_counter}")

cap.release()
cv2.destroyAllWindows()