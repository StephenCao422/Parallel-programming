#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>

// Function to compress an image using multiple threads
void compressImage(const cv::Mat& inputImage,
                   cv::Mat& outputImage,
                   int numThreads,
                   int quality) {
    int imageSize = inputImage.total();
    int numChannels = inputImage.channels();
    int chunkSize = imageSize / numThreads;

    // Vector to hold threads
    std::vector<std::thread> threads(numThreads);

    // Function to run in each thread
    auto compressChunk = [&](int startIndex) {
        // Compress a chunk of the input image
        for (int i = startIndex; i < startIndex + chunkSize; i++) {
            outputImage.at<double>(i) = inputImage.at<uchar>(i) / quality;
        }
    };

    // Launch threads
    for (int i = 0; i < numThreads; i++) {
        threads[i] = std::thread(compressChunk, i * chunkSize * numChannels);
    }

    // Join threads
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
}

int main() {
    // Read input image and convert it to double precision floating-point data type
    cv::Mat inputImage = cv::imread("/Users/stephencao/Downloads/1.png", cv::IMREAD_GRAYSCALE);
    inputImage.convertTo(inputImage, CV_64FC1);

    // DCT transform
    cv::Mat dctImage;
    cv::dct(inputImage, dctImage);

    // Quantization matrix
    cv::Mat quantizationMatrix = cv::Mat::ones(8, 8, CV_64FC1) * 10;

    // Quantization
    cv::Mat quantizedImage = dctImage / quantizationMatrix;

    // Output image
    cv::Mat outputImage;
    cv::idct(quantizedImage, outputImage);

    // Convert the output image back to 8-bit unsigned integer data type
    outputImage.convertTo(outputImage, CV_8UC1);

    // Write compressed image
    cv::imwrite("output_image.jpg", outputImage);

    return 0;
}
