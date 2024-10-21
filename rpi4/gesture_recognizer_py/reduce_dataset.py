import os
import random

# ASL dataset was too big for my usecase

# generated with chatpgt

# Directory containing the ASL dataset
asl_dataset_train_dir = 'dataset/train'

# Number of images to keep per class
images_to_keep = 500

# Loop through each class folder (e.g., 'A', 'B', 'C', etc.)
for label in os.listdir(asl_dataset_train_dir):
    label_dir = os.path.join(asl_dataset_train_dir, label)

    # Check if it's a directory
    if not os.path.isdir(label_dir):
        continue

    # Get a list of all image files in the class folder
    images = os.listdir(label_dir)

    # Check if the number of images is greater than the required amount to keep
    if len(images) > images_to_keep:
        # Randomly select the images to delete
        images_to_remove = random.sample(images, len(images) - images_to_keep)

        # Remove the selected images
        for img_file in images_to_remove:
            img_path = os.path.join(label_dir, img_file)
            if os.path.exists(img_path):
                os.remove(img_path)
                print(f"Removed: {img_path}")

print("Image reduction complete.")
