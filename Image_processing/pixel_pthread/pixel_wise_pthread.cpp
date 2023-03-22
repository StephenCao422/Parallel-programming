#include <iostream>
#include <vector>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <pthread.h>

using namespace cv;

struct ThreadData {
    const Mat* inputImage;
    Mat* outputImage;
    int startRow;
    int endRow;
};

void* processImage(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    const Mat& inputImage = *(data->inputImage);
    Mat& outputImage = *(data->outputImage);
    int startRow = data->startRow;
    int endRow = data->endRow;
    for (int i = startRow; i < endRow; ++i) {
        for (int j = 0; j < inputImage.cols; ++j) {
            Vec3b pixel = inputImage.at<Vec3b>(i, j);
            pixel[0] *= 3; // double blue channel
            pixel[1] *= 3; // double green channel
            pixel[2] *= 3; // double red channel
            outputImage.at<Vec3b>(i, j) = pixel;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv)
{
    // Read input image
    Mat inputImage = imread("/Users/stephencao/Downloads/1.png", IMREAD_COLOR);
    cv::resize(inputImage, inputImage, cv::Size(1000, 1000));
    // Create output image
    Mat outputImage = Mat::zeros(inputImage.size(), inputImage.type());

    // Partition the image into 4 regions
    int numRegions = 4;
    int rowsPerRegion = inputImage.rows / numRegions;

    // Create threads to process each region
    std::vector<pthread_t> threads(numRegions);
    std::vector<ThreadData> threadData(numRegions);
    auto start = std::chrono::high_resolution_clock::now(); // start measuring time
    for (int i = 0; i < numRegions; ++i) {
        int startRow = i * rowsPerRegion;
        int endRow = (i + 1) * rowsPerRegion;
        if (i == numRegions - 1) {
            endRow = inputImage.rows;
        }
        threadData[i] = {&inputImage, &outputImage, startRow, endRow};
        pthread_create(&threads[i], NULL, processImage, &threadData[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numRegions; ++i) {
        pthread_join(threads[i], NULL);
    }
    auto end = std::chrono::high_resolution_clock::now(); // end measuring time
    std::chrono::duration<double> elapsed = end - start; // calculate elapsed time in seconds
    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;

    // Display output image
    imshow("Output", outputImage);
    waitKey(0);

    return 0;
}
