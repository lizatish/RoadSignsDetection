#ifndef SIGNTRAINER_H
#define SIGNTRAINER_H

// Standard library
#include <iostream>
#include <vector>
#include <string>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

// TinyDir
#include <tinydir.h>

//Qt
#include "QDebug"
#include <iostream>

using namespace std;
using namespace cv::ml;
using namespace cv;

class signTrainer
{
public:
    signTrainer();
    void performTraining();

private:
    std::vector<int> labels;
    std::vector<std::string> trainingFilenames;

    void getsTrainingFiles();

};

#endif // SIGNTRAINER_H
