############################################
## Author: Michal Balogh, xbalog06        ##
## Date: 14.10.2024                       ##
############################################

# Script for reducing the number of images in the ASL dataset
# ASL dataset was too big for my usecase

import os
import random

asl_dataset_train_dir = 'dataset/train'

images_to_keep = 500

# Loop through each class folder (e.g., 'A', 'B', 'C', etc.)
for label in os.listdir(asl_dataset_train_dir):
    label_dir = os.path.join(asl_dataset_train_dir, label)

    if not os.path.isdir(label_dir):
        continue

    # All images in the current class folder
    images = os.listdir(label_dir)

    # Loop until desired number of images is reached
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
