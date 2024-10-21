############################################
## Author: Michal Balogh, xbalog06        ##
## Date: 14.10.2024                       ##
############################################

import torch
from torch.utils.data import Dataset, DataLoader
import pandas as pd

# pytorch dataset class for the ASL hand landmarks dataset

class ASLLandmarkDataset(Dataset):
    def __init__(self, csv_file):
        self.data = pd.read_csv(csv_file)
        self.labels = self.data['label']
        self.landmarks = self.data.drop(columns=['label']).values

    def __len__(self):
        return len(self.landmarks)

    def __getitem__(self, idx):
        landmarks = torch.tensor(self.landmarks[idx], dtype=torch.float32)

        # if label is del, nothing or space, assign it to 26, 27, 28
        if self.labels[idx] == 'del':
            label = 26
        elif self.labels[idx] == 'space':
            label = 27
        else:
            label = ord(self.labels[idx]) - ord('A')
        label = torch.tensor(label, dtype=torch.long)

        return landmarks, label


if __name__ == '__main__':
    csv_file = "dataset/asl_landmarks_dataset.csv"
    landmark_dataset = ASLLandmarkDataset(csv_file)
    dataloader = DataLoader(landmark_dataset, batch_size=32, shuffle=True)

    # example of one batch
    for landmarks, label in dataloader:
        print(f'Landmarks shape: {landmarks.shape}, Label: {label}')
        break
