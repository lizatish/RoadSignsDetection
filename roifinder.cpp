#include "roifinder.h"

roiFinder::roi roiFinder::startSearch(const Mat& frame){

    Mat hsvFrame;
    GaussianBlur(frame, frame, Size(5, 5), 2, 2); //размытие по Гауссу
    cvtColor(frame, hsvFrame, CV_BGR2HSV);

    // Поиск красного
    Mat mask1Red, mask2Red;
    roiFinder::hsvParams hsvRed1 = { 0, 16, 110, 255, 130, 255};
    roiFinder::hsvParams hsvRed2 = { 170, 180, 110, 255, 130, 255};
    hsvInRange(hsvFrame, mask1Red, hsvRed1);
    hsvInRange(hsvFrame, mask2Red, hsvRed2);
    Mat maskRed = mask1Red | mask2Red;
    imshow("Red", maskRed);

    // Поиск синего
    Mat maskBlue;
    roiFinder::hsvParams hsvBlue = { 100, 120, 140, 255, 40, 255};
    hsvInRange(hsvFrame, maskBlue, hsvBlue);
    imshow("Blue", maskBlue);

    // Обработка hsv изображения
    Mat binaryRed, binaryBlue;
    processingBinaryImage(maskRed, binaryRed);
    processingBinaryImage(maskBlue, binaryBlue);
    //        imshow("1", binaryRed);
    //        imshow("2", binaryBlue);

    // Нахождение областей ROI
    Mat ROIRedFrame,  ROIBlueFrame;
    vector<Rect> boundRectRed =  findROI(frame, binaryRed, ROIRedFrame);
    vector<Rect> boundRectBlue =  findROI(frame, binaryBlue, ROIBlueFrame);

    // Поиск кругов на ROI
    vector<Mat> roiRed =  isItCircle(binaryRed, boundRectRed);
    vector<Mat> roiBlue =  isItCircle(binaryBlue, boundRectBlue);

    //    vector<Mat> roiBlue;
    //    vector<Mat> roiRed;
    //    for(uint i = 0; i < boundRectRed.size(); i++){
    //        Mat testMat = binaryRed(boundRectRed[i]).clone();
    //        roiRed.push_back(testMat);
    //    }
    //    for(uint i = 0; i < boundRectBlue.size(); i++){
    //        Mat testMat = binaryBlue(boundRectBlue[i]).clone();
    //        roiBlue.push_back(testMat);
    //    }

    roiFinder::roi r;
    r.roiR = roiRed;
    r.roiB = roiBlue;
    return r;
}

// Обработка бинаризованного изображения(удаление шумов)
void roiFinder::processingBinaryImage(const Mat& src, Mat& dst){

    //Морфологическое размыкание
    Mat morphologyOpenImg, element;
    element = Mat();
    morphologyEx(src, morphologyOpenImg, MORPH_OPEN, element);

    // Медианное сглаживание

    //    Mat dst_median;
    //    //Ядро
    //    int kernel_size = 3;
    //    medianBlur (morphologyOpenImg,  dst_median, kernel_size );

    Mat erosion_dst;
    //Эрозия
    int erosion_size = 1;
    element = getStructuringElement( MORPH_RECT,
                                     Size( 1*erosion_size + 1, 1*erosion_size+1 ),
                                     Point( erosion_size, erosion_size ) );
    erode( morphologyOpenImg, erosion_dst, element );

    //Дилатация
    int dilation_size = 1;
    element = getStructuringElement( MORPH_RECT,
                                     Size( 1*dilation_size + 1, 1*dilation_size+1 ),
                                     Point( dilation_size, dilation_size ) );
    dilate( erosion_dst, dst, element );
}

vector<Rect> roiFinder::findROI(const Mat& frame, const Mat& binary_frame, Mat& outputFrame){

    vector<vector<Point> > contours;    // Вектор точек контура
    vector<Vec4i> hierarchy;            // Вектор иерархий контуров

    Mat morph_frame = binary_frame.clone();
    findContours( morph_frame, contours, hierarchy,
                  CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) ); //Поиск контуров и запись их

    // Если есть контура
    vector<Rect> boundRect( contours.size() );
    if(contours.size() > 0)
    {
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Point2f> center( contours.size() );
        vector<float> radius( contours.size() );

        // Создание областей интересов и запись их в boundRect
        for(unsigned int m = 0; m < contours.size(); m++ )
        {
            approxPolyDP( Mat(contours[m]), contours_poly[m], 3, true );
            boundRect[m] = boundingRect( Mat(contours_poly[m]) );
            minEnclosingCircle( (Mat)contours_poly[m], center[m], radius[m] );
        }
    }

    /////////// Удаление ненужных областей///////////////
    // Удаление маленьких областей ROI
    for(unsigned int k = 0; k < boundRect.size(); k++ )
    {
        if((boundRect[k].width < 30)||(boundRect[k].height < 30)){
            boundRect.erase(boundRect.begin() + k);
            k--;
        }
        else if ((boundRect[k].width >= boundRect[k].height * 2)||(boundRect[k].height >= 2 * boundRect[k].width))
        {
            boundRect.erase(boundRect.begin() + k);
            k--;
        }
        else if((boundRect[k].width > frame.rows - 15)||(boundRect[k].height > frame.cols - 15)){
            boundRect.erase(boundRect.begin() + k);
            k--;
        }
    }

    // Удаление неквадратных областей
    for(unsigned int k = 0; k < boundRect.size(); k++ ){
        if(abs(boundRect[k].width - boundRect[k].height) > 35){
            boundRect.erase(boundRect.begin() + k);
            k--;
        }
    }

    // Удаление вложенных многоугольников
    for(unsigned int i = 0; i < boundRect.size(); i++){
        for(unsigned int j = 0; j < boundRect.size(); j++){
            if(i != j) {
                Rect intersection = boundRect[i] & boundRect[j];
                if(intersection.area()) {
                    if(boundRect[i].area() > boundRect[j].area() ) {
                        boundRect.erase(boundRect.begin() + j);
                        j--;
                    }
                }
            }
        }
    }
    outputFrame = frame.clone();
    // Рисование прямоугольников - зон интересов
    for(unsigned int k = 0; k < boundRect.size(); k++ )
    {
        Scalar color = Scalar(0, 120, 255);
        rectangle( outputFrame, boundRect[k].tl(), boundRect[k].br(), color, 2, 8, 0 );
    }

    return boundRect;
}
void roiFinder::hsvInRange(const Mat& src, Mat& dst, hsvParams hsvParam){
    inRange(src, Scalar(hsvParam.hDown, hsvParam.sDown, hsvParam.vDown),
            Scalar(hsvParam.hUp, hsvParam.sUp, hsvParam.vUp), dst);

}

vector<Mat> roiFinder::isItCircle(const Mat& binaryFrame, vector<Rect> boundRect){

    vector<Mat> roi;
    //Поиск кругов
    vector<Vec3f> circles;

    for(uint i = 0; i < boundRect.size(); i++){
        Mat testMat = binaryFrame(boundRect[i]).clone();
        imshow("roiBEFORE", testMat);
        HoughCircles(testMat, circles, 3,
                     15,  // accumulator resolution (size of the image / 2)
                     testMat.cols,   // minimum distance between two circles
                     100, // Canny high threshold
                     200, // minimum number of votes
                     testMat.cols/3,
                     testMat.cols/2 - 10); // min and max radius

        if(circles.size() > 0){
            for(uint m = 0; m < circles.size(); m++ )
                roi.push_back(testMat);

        }
    }
    return roi;
}


