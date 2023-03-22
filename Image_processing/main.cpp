#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <opencv2/opencv.hpp>

using namespace cv;

#define NUM_THREADS 8

struct thread_data {
    int thread_id;
    int rows_per_thread;
    Mat *image;
};

void *grayscale_filter(void *threadarg) {
    struct thread_data *data;
    data = (struct thread_data *) threadarg;

    int start_row = data->thread_id * data->rows_per_thread;
    int end_row = (data->thread_id + 1) * data->rows_per_thread;

    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < data->image->cols; j++) {
            Vec3b pixel = data->image->at<Vec3b>(i, j);
            int gray_value = 0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0];
            data->image->at<Vec3b>(i, j) = Vec3b(gray_value, gray_value, gray_value);
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <image_file>" << std::endl;
        return EXIT_FAILURE;
    }

    Mat image = imread("/Users/stephencao/Downloads/1.png", IMREAD_COLOR);

    if (image.empty()) {
        std::cerr << "Failed to read image file " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    int num_rows = image.rows;
    int rows_per_thread = num_rows / NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data_array[i].thread_id = i;
        thread_data_array[i].rows_per_thread = rows_per_thread;
        thread_data_array[i].image = &image;
        int rc = pthread_create(&threads[i], NULL, grayscale_filter, (void *) &thread_data_array[i]);
        if (rc) {
            std::cerr << "Error:unable to create thread," << rc << std::endl;
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    imwrite("/Users/stephencao/Downloads/1.png", image);
    //imshow("Grayscale Image", image);
    //waitKey(0);//

    return 0;
}
