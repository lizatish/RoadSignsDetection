#include "signclassifier.h"
using namespace cv;

signClassifier::signClassifier()
{
    svm = SVM::load("/home/liza/Downloads/Signs/classifier.yml");

}



int signClassifier::detectDigit(const Mat& imgMat) {

    Mat testMat;
    imshow("ROI", imgMat);
    getSquareImage(imgMat, testMat, 35);

    testMat = testMat.reshape(1,1);
    testMat.convertTo(testMat, CV_32FC1);
    predictedNumber =svm->predict(testMat);
    return predictedNumber;
}

int signClassifier::getPredictedNumber() {
    int PN = predictedNumber;
    return PN;
}

// Грамотное получение квадратного ROI из прямоугольного
void signClassifier::getSquareImage( const Mat& src, Mat& dstSquare, int target_width )
{
    int width = src.cols,
            height = src.rows;

    dstSquare = Mat::zeros( target_width, target_width, src.type() );

    int max_dim = ( width >= height ) ? width : height;
    float scale = ( ( float ) target_width ) / max_dim;
    Rect roi;

    if ( width >= height )
    {
        roi.width = target_width;
        roi.x = 0;
        roi.height = height * scale;
        roi.y = ( target_width - roi.height ) / 2;
    }
    else
    {
        roi.y = 0;
        roi.height = target_width;
        roi.width = width * scale;
        roi.x = ( target_width - roi.width ) / 2;
    }

    resize( src, dstSquare( roi ), roi.size() );
}
