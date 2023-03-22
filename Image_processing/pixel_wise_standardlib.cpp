#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void processImage(const Mat& input, Mat& output) {
    for (int i = 0; i < input.rows; i++) {
        const uchar* p = input.ptr<const uchar>(i);
        uchar* q = output.ptr<uchar>(i);
        for (int j = 0; j < input.cols * input.channels(); j++) {
            q[j] = p[j] * 2;
        }
    }
}


int main() {
    Mat image = imread("/Users/stephencao/Downloads/1.png", IMREAD_COLOR);
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output(image.rows, image.cols, image.type());

    thread t(processImage, cref(image), ref(output));
    t.join();

    namedWindow("Output Image", WINDOW_NORMAL);
    imshow("Output Image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
