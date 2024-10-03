#include <iostream>
#include <opencv2/opencv.hpp>
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/solutions/hands.h"

namespace mp = mediapipe;

int main(int argc, char** argv) {
    // Initialize MediaPipe Hands solution.
    mp::Hands hands;
    hands.Initialize(true, 0.3); // Static mode, min detection confidence

    // Load the image using OpenCV.
    cv::Mat image = cv::imread("demo_img/hand.jpg");
    if (image.empty()) {
        std::cerr << "Could not read the image." << std::endl;
        return -1;
    }

    // Convert the image to RGB as required by MediaPipe.
    cv::Mat image_rgb;
    cv::cvtColor(image, image_rgb, cv::COLOR_BGR2RGB);

    // Process the image to find hand landmarks.
    auto results = hands.Process(image_rgb);

    if (!results.multi_hand_landmarks.empty()) {
        for (const auto& hand_landmarks : results.multi_hand_landmarks) {
            // Draw the landmarks on the image using OpenCV.
            for (int i = 0; i < hand_landmarks.landmark_size(); ++i) {
                const auto& lm = hand_landmarks.landmark(i);
                cv::circle(image, cv::Point(lm.x() * image.cols, lm.y() * image.rows), 5, cv::Scalar(0, 255, 0), -1);
            }

            // Print out the landmarks.
            for (int i = 0; i < hand_landmarks.landmark_size(); ++i) {
                const auto& lm = hand_landmarks.landmark(i);
                std::cout << "Landmark " << i << ": (" << lm.x() << ", " << lm.y() << ", " << lm.z() << ")" << std::endl;
            }
        }
    }

    // Show the image with landmarks drawn on it.
    cv::imshow("Hand Landmarks", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}
