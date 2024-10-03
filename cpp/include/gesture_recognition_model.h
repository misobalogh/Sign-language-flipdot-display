#ifndef GESTURE_RECOGNITION_MODEL_H
#define GESTURE_RECOGNITION_MODEL_H

#include <torch/torch.h>

struct GestureRecognitionModel : torch::nn::Module {
    torch::nn::Sequential recognizer;

    GestureRecognitionModel(int input_size, int num_classes) {
        recognizer = register_module("recognizer", torch::nn::Sequential(
            torch::nn::Linear(input_size, 128),
            torch::nn::ReLU(),
            torch::nn::Linear(128, 64),
            torch::nn::ReLU(),
            torch::nn::Linear(64, num_classes)
        ));
    }

    torch::Tensor forward(torch::Tensor x) {
        return recognizer->forward(x);
    }
};

#endif // GESTURE_RECOGNITION_MODEL_H
