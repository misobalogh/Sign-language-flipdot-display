import torch
import torch.jit

from gesture_recognition_model import GestureRecognitionModel

model = GestureRecognitionModel(input_size=63, num_classes=28)
model.load_state_dict(torch.load("gesture_recognition_model.pth"))
model.eval()
scripted_model = torch.jit.script(model)
scripted_model.save("gesture_recognition_model.pt")
