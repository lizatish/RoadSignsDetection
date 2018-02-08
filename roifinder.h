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
    struct hsvParams{
        int hDown;
        int hUp;
        int sDown;
        int sUp;
        int vDown;
        int vUp;
    };
    struct roi{
        vector<Mat> roiR;
        vector<Mat> roiB;
    };

    roiFinder::roi startSearch(const Mat&);
    void processingBinaryImage(const Mat&, Mat&);
    void hsvInRange(const Mat&, Mat&, hsvParams);
    vector<Rect> findROI(const Mat&, const Mat&, Mat&);
    vector<Mat> isItCircle(const Mat&, vector<Rect>);

};

#endif // ROIFINDER_H
