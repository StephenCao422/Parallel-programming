#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;

// Sobel kernel for detecting edges in x and y direction
int sobel_x_kernel[3][3] = {{-1, 0, 1},
                            {-2, 0, 2},
                            {-1, 0, 1}};

int sobel_y_kernel[3][3] = {{-1, -2, -1},
                            { 0,  0,  0},
                            { 1,  2,  1}};

// Function that processes a sub-region of the image
void processSubRegion(Mat image, Mat& output, int startX, int endX, int startY, int endY)
{
    // Iterate through pixels in sub-region
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            // Apply Sobel operator to obtain gradient in x and y direction
            int gradient_x = 0, gradient_y = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    int pixel_x = x + i - 1;
                    int pixel_y = y + j - 1;
                    if (pixel_x >= 0 && pixel_x < image.cols && pixel_y >= 0 && pixel_y < image.rows) {
                        gradient_x += image.at<uchar>(pixel_y, pixel_x) * sobel_x_kernel[i][j];
                        gradient_y += image.at<uchar>(pixel_y, pixel_x) * sobel_y_kernel[i][j];
                    }
                }
            }

            // Compute magnitude of gradient and threshold to obtain binary image
            int magnitude = sqrt(pow(gradient_x, 2) + pow(gradient_y, 2));
            if (magnitude > 128) {
                output.at<uchar>(y, x) = 255;
            } else {
                output.at<uchar>(y, x) = 0;
            }
        }
    }
}

int main() {
    // Load image and convert to grayscale
    Mat image = imread("/Users/stephencao/Downloads/4.JPG", IMREAD_GRAYSCALE);
    cv::resize(image, image, cv::Size(1000, 1000));
    // Split image into 4 sub-regions
    int num_threads = 4;
    int sub_width = image.cols / num_threads;
    std::vector<std::thread> threads;
    std::vector<Mat> outputs(num_threads, Mat::zeros(image.rows, sub_width, CV_8UC1));

    // Start the timer
    auto start_time = std::chrono::high_resolution_clock::now();

    // Process sub-regions using threads
    for (int i = 0; i < num_threads; i++) {
        int startX = i * sub_width;
        int endX = (i + 1) * sub_width;
        int startY = 0;
        int endY = image.rows;
        if (i == num_threads - 1) {
            endX = image.cols;
        }
        threads.push_back(std::thread(processSubRegion, image, std::ref(outputs[i]), startX, endX, startY, endY));
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }


// Combine sub-region results to get final output image
    Mat output(image.rows, image.cols, CV_8UC1);
    for (int i = 0; i < num_threads; i++) {
        int startX = i * sub_width;
        int endX = (i + 1) * sub_width;
        if (i == num_threads - 1) {
            endX = image.cols;
        }
        Rect roi(startX, 0, endX - startX, image.rows);
        outputs[i].copyTo(output(roi));
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration_ms.count() << " ms" << std::endl;
// Display output image
    namedWindow("Output", WINDOW_NORMAL);
    imshow("Output", output);
    waitKey(0);

    return 0;

}