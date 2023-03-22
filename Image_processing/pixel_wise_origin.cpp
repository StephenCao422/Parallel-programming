#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("/Users/stephencao/Downloads/1.png", IMREAD_COLOR);
    if (image.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat output(image.rows, image.cols, image.type());

    for (int i = 0; i < image.rows; i++) {
        uchar* p = image.ptr(i);
        uchar* q = output.ptr(i);
        for (int j = 0; j < image.cols * image.channels(); j++) {
            q[j] = p[j] * 2;
        }
    }

    //imwrite("/Users/stephencao/Downloads/1.png", output);
    namedWindow("Output Image", WINDOW_NORMAL);
    imshow("Output Image", output);

    waitKey(0);
    destroyAllWindows();

    return 0;
}
