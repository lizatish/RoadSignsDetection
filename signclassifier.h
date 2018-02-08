#ifndef SIGNCLASSIFIER_H
#define SIGNCLASSIFIER_H

// Standard library
#include <iostream>
#include <vector>
#include <string>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv/ml.h>

// TinyDir
#include "tinydir.h"

//Qt
#include "QDebug"
#include <iostream>

using namespace std;
using namespace cv;
using namespace cv::ml;

class signClassifier
{
public:
    signClassifier();
    int detectDigit(const Mat& imgMat);
    int getPredictedNumber();
    void getSquareImage(const Mat&, Mat&, int);
private:
    Ptr<SVM> svm;
    int predictedNumber;
};

#endif // SIGNCLASSIFIER_H
