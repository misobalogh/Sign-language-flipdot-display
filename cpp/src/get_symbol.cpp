#include <iostream>
#include <opencv2/opencv.hpp>
#include <torch/script.h> 
#include <torch/torch.h>
#include <map>
#include "gesture_recognition_model.h" 

std::map<int, std::string> labels_mapping = {
    {0, "A"}, {1, "B"}, {2, "C"}, {3, "D"}, {4, "E"},
    {5, "F"}, {6, "G"}, {7, "H"}, {8, "I"}, {9, "J"},
    {10, "K"}, {11, "L"}, {12, "M"}, {13, "N"}, {14, "O"},
    {15, "P"}, {16, "Q"}, {17, "R"}, {18, "S"}, {19, "T"},
    {20, "U"}, {21, "V"}, {22, "W"}, {23, "X"}, {24, "Y"},
    {25, "Z"}, {26, "del"}, {27, "space"}
};

std::string predict_gesture(torch::Tensor& landmarks_tensor, GestureRecognitionModel& model) {
    torch::NoGradGuard no_grad; // Disable gradient computation
    auto output = model.forward(landmarks_tensor);
    auto predicted = std::get<1>(output.max(1)); // Get index of max value
    int predicted_index = predicted.item<int>();
    
    return labels_mapping.count(predicted_index) ? labels_mapping[predicted_index] : "Unknown";
}

int get_symbols_from_cam() {
    int input_size = 63; // 21 landmarks * 3 coordinates (x, y, z)
    int num_classes = 28; // 26 letters + 2 special characters

    // Load the model in TorchScript format
    GestureRecognitionModel model(input_size, num_classes);
    torch::load(model, "gesture_recognition_model.pt");
    
    // Open video capture
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open camera." << std::endl;
        return -1;
    }
    
    // Hand landmark detection setup
    auto hands = mp_hands::Hands(static_image_mode=false, min_detection_confidence=0.3, min_tracking_confidence=0.3);

    while (true) {
        cv::Mat frame;
        cap >> frame; // Capture frame

        // Process the image for hand landmarks
        cv::Mat image_rgb;
        cv::cvtColor(frame, image_rgb, cv::COLOR_BGR2RGB);
        auto results = hands.process(image_rgb);

        if (results.multi_hand_landmarks) {
            for (const auto& hand_landmarks : results.multi_hand_landmarks) {
                // Draw landmarks on the image
                mp_drawing::draw_landmarks(frame, hand_landmarks, mp_hands::HAND_CONNECTIONS);
                
                // Flatten the landmarks to 1D array
                std::vector<float> landmarks;
                for (const auto& lm : hand_landmarks.landmark) {
                    landmarks.push_back(lm.x);
                    landmarks.push_back(lm.y);
                    landmarks.push_back(lm.z);
                }

                // Convert landmarks to tensor
                auto landmarks_tensor = torch::tensor(landmarks, torch::dtype(torch::kFloat32)).unsqueeze(0); // Add batch dimension

                std::string gesture = predict_gesture(landmarks_tensor, model);
                std::cout << gesture << std::endl;
            }
        }

        cv::imshow("Gesture Recognition", frame);
        if (cv::waitKey(1) == 'q') break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
