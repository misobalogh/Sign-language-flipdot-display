############################################
## Author: Michal Balogh, xbalog06        ##
## Date: 14.10.2024                       ##
############################################

# Simple model for gesture recognition from 3D hand landmarks

import torch.nn as nn

class GestureRecognitionModel(nn.Module):
    def __init__(self, input_size, num_classes):
        super(GestureRecognitionModel, self).__init__()
        self.recognizer = nn.Sequential(
            nn.Linear(input_size, 128),
            nn.BatchNorm1d(128),
            nn.ReLU(),
            nn.Dropout(0.25),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Linear(64, num_classes)
        )

    def forward(self, x):
        return self.recognizer(x)


if __name__ == '__main__':
    input_size = 63 # number of landmarks (21 for each x, y, z)
    num_classes = 28 # number of classes (26 alphabets + 2 special characters)
    model = GestureRecognitionModel(input_size=input_size, num_classes=num_classes)
    print(model)
