#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

void processImage(const Mat& inputImage, Mat& outputImage)
{
    for (int i = 0; i < inputImage.rows; ++i) {
        for (int j = 0; j < inputImage.cols; ++j) {
            Vec3b pixel = inputImage.at<Vec3b>(i, j);
            pixel[0] *= 3; // double blue channel
            pixel[1] *= 3; // double green channel
            pixel[2] *= 3; // double red channel
            outputImage.at<Vec3b>(i, j) = pixel;
        }
    }
}

int main(int argc, char** argv)
{
    // Read input image
    Mat inputImage = imread("/Users/stephencao/Downloads/1.png", IMREAD_COLOR);
    //cv::resize(inputImage, inputImage, cv::Size(10000, 10000));
    // Create output image
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // Start the clock
    auto start = std::chrono::high_resolution_clock::now();

    // Process image
    processImage(inputImage, outputImage);

    // Stop the clock
    auto stop = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    // Output the elapsed time
    std::cout << "Elapsed time: " << duration.count() << " microseconds." << std::endl;

    // Get number of pixels in the image
    int numPixels = inputImage.total();

    // Print number of pixels
    std::cout << "Number of pixels: " << numPixels << std::endl;
    // Display output image
    imshow("Output", outputImage);
    waitKey(0);

    return 0;
}
