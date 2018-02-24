#ifndef ROADSIGNSDETECTION_H
#define ROADSIGNSDETECTION_H

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

#include "signclassifier.h"
#include "signtrainer.h"
#include "roifinder.h"

class roadSignsDetection
{
private:
    roiFinder::hsvParams hsvRed1 = { 0, 16, 110, 255, 130, 255};
    roiFinder::hsvParams hsvRed2 = { 170, 180, 110, 255, 130, 255};
    roiFinder::hsvParams hsvBlue = { 100, 120, 140, 255, 40, 255};

public:
    void startSignsDetection();
    void startTraining();
    void startClassifier(const Mat&);
    vector<Mat> startFindRoi(Mat& frame, roiFinder::hsvParams p1, roiFinder::hsvParams p2);
};

#endif // ROADSIGNSDETECTION_H
