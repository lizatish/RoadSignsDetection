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
public:
    void startSignsDetection();
    void startTraining();
    void startClassifier(const Mat&);
    roiFinder::roi startFindRoi(Mat& frame);
};

#endif // ROADSIGNSDETECTION_H
