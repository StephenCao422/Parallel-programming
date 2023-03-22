#include <iostream>
#include <cmath>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

// struct to hold thread arguments
struct thread_data {
    Mat inputImage;
    Mat outputImage;
    int start_row;
    int end_row;
    int width;
    int height;
    int cx;
    int cy;
    float twistIntensity;
};

// function to perform twist transformation on image
void* TwistTransform(void* arg) {
    thread_data* data = (thread_data*) arg;
    Mat inputImage = data->inputImage;
    Mat outputImage = data->outputImage;
    int width = data->width;
    int height = data->height;
    int cx = data->cx;
    int cy = data->cy;
    float twistIntensity = data->twistIntensity;

    for (int y = data->start_row; y < data->end_row; y++) {
        for (int x = 0; x < width; x++) {
            int index = y * width + x;
            float radius = sqrt(pow(x - cx, 2) + pow(y - cy, 2));
            float theta = (radius / 1.5) * twistIntensity;
            float x_prime = cos(theta) * (x - cx) - sin(theta) * (y - cy) + cx;
            float y_prime = -sin(theta) * (x - cx) + cos(theta) * (y - cy) + cy;
            if (x_prime < 0 || x_prime >= width || y_prime < 0 || y_prime >= height) {
                outputImage.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
            }
            else {
                outputImage.at<Vec3b>(y, x) = inputImage.at<Vec3b>(y_prime, x_prime);
            }
        }
    }
    pthread_exit(NULL);
}

int main() {
    // read image
    Mat inputImage = imread("/Users/stephencao/Downloads/4.JPG", IMREAD_COLOR);
    cv::resize(inputImage, inputImage, cv::Size(1000, 1000));
    if (inputImage.empty()) {
        cerr << "Error: Could not read image" << endl;
        return -1;
    }

    // set twist parameters
    int cx = inputImage.cols / 2;
    int cy = inputImage.rows / 2;
    float twistIntensity = 0.02;

    // create output image
    Mat outputImage(inputImage.rows, inputImage.cols, CV_8UC3);

    // set number of threads to use
    int num_threads = 4;

    // create thread data and threads
    thread_data thread_args[num_threads];
    pthread_t threads[num_threads];
    int rows_per_thread = ceil((float) inputImage.rows / num_threads);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; i++) {
        thread_args[i].inputImage = inputImage;
        thread_args[i].outputImage = outputImage;
        thread_args[i].start_row = i * rows_per_thread;
        thread_args[i].end_row = min((i + 1) * rows_per_thread, inputImage.rows);
        thread_args[i].width = inputImage.cols;
        thread_args[i].height = inputImage.rows;
        thread_args[i].cx = cx;
        thread_args[i].cy = cy;
        thread_args[i].twistIntensity = twistIntensity;
        pthread_create(&threads[i], NULL, TwistTransform, (void *) &thread_args[i]);
    }

    // join threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Execution time: " << duration_ms.count() << " ms" << std::endl;

    namedWindow("outputImage", WINDOW_NORMAL);
    imshow("outputImage", outputImage);
    waitKey(0);
    return 0;
}