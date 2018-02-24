#ifndef ROIFINDER_H
#define ROIFINDER_H

#include <iostream>
#include <string>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/ml.h>
using namespace cv;

#include <tinydir.h>
#include <clocale>
#include <wchar.h>
#include <QTextCodec>
#include <QByteArray>
#include <QDebug>


class roiFinder
{

public:
    roiFinder();
    struct hsvParams{
        int hDown;
        int hUp;
        int sDown;
        int sUp;
        int vDown;
        int vUp;
    };
    vector<Mat> startSearch(const Mat& frame, hsvParams params1, hsvParams params2);
    void processingBinaryImage(const Mat&, Mat&);
    void hsvInRange(const Mat&, Mat&, hsvParams);
    vector<Rect> findROI(const Mat&, const Mat&);
    vector<Mat> isItCircle(const Mat&, vector<Rect>);
};


#endif // ROIFINDER_H
