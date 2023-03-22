#include <iostream>
#include <thread>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void detectObjects(Mat frame, vector<Rect>& objects)
{
    CascadeClassifier cascade;
    cascade.load("haarcascade_frontalface_alt.xml");

    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    cascade.detectMultiScale(gray, objects, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
}

int main()
{
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Could not open camera" << endl;
        return -1;
    }

    namedWindow("Camera", WINDOW_NORMAL);

    vector<Rect> objects;

    while (true) {
        Mat frame;
        cap >> frame;

        thread detectionThread(detectObjects, frame, ref(objects));
        detectionThread.join();

        for (size_t i = 0; i < objects.size(); i++) {
            rectangle(frame, objects[i], Scalar(0, 255, 0));
        }

        imshow("Camera", frame);
        if (waitKey(10) == 27) break;
    }

    destroyAllWindows();
    return 0;
}
