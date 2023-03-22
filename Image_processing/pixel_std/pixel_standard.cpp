#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace cv;

void processImage(const Mat& inputImage, Mat& outputImage, int startRow, int endRow)
{
    for (int i = startRow; i < endRow; ++i) {
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
    cv::resize(inputImage, inputImage, cv::Size(1000, 1000));
    // Create output image
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // Partition the image into 4 regions
    int numRegions = 2;
    int rowsPerRegion = inputImage.rows / numRegions;

    // Create threads to process each region
    std::vector<std::thread> threads(numRegions);
    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < numRegions; ++i) {
        int startRow = i * rowsPerRegion;
        int endRow = (i + 1) * rowsPerRegion;
        if (i == numRegions - 1) {
            endRow = inputImage.rows;
        }
        threads[i] = std::thread(processImage, std::ref(inputImage), std::ref(outputImage), startRow, endRow);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
    auto end = std::chrono::high_resolution_clock::now(); // end measuring time
    std::chrono::duration<double> elapsed = end - start; // calculate elapsed time in seconds
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;

    // Display output image
    imshow("Output", outputImage);
    waitKey(0);

    return 0;
}
