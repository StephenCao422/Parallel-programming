#include <iostream>
#include <opencv2/opencv.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace cv;

void processImage(const Mat& input, Mat& output, int startRow, int endRow) {
    for (int i = startRow; i < endRow; i++) {
        const uchar* p = input.ptr<const uchar>(i);
        uchar* q = output.ptr<uchar>(i);
        for (int j = 0; j < input.cols * input.channels(); j++) {
            q[j] = p[j] * 2;
        }
    }
}

int main() {
    Mat image = imread("input.jpg", IMREAD_COLOR);
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output(image.rows, image.cols, image.type());

    const int numThreads = boost::thread::hardware_concurrency();
    vector<boost::thread> threads(numThreads);

    const int rowsPerThread = image.rows / numThreads;

    int startRow = 0;
    for (int i = 0; i < numThreads; i++) {
        int endRow = startRow + rowsPerThread;
        if (i == numThreads - 1) {
            endRow = image.rows;
        }
        threads[i] = boost::thread(processImage, boost::ref(image), boost::ref(output), startRow, endRow);
        startRow = endRow;
    }

    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }

    namedWindow("Output Image", WINDOW_NORMAL);
    imshow("Output Image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}

