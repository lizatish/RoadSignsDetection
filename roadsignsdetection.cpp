#include "roadsignsdetection.h"

void roadSignsDetection::startSignsDetection(){

    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        exit(1);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640); //установка ширины получаемого кадра
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480); //Установка высоты получаемого кадра

    roadSignsDetection* RD;
    RD = new roadSignsDetection;

    while(true){

        Mat frame;
        cap >> frame;
        imshow("Src", frame);

        roiFinder::hsvParams hsv0 = {0, 0, 0, 0, 0, 0,};
        vector<Mat> roiR = RD->startFindRoi(frame, hsvRed1, hsvRed2);
        vector<Mat> roiB = RD->startFindRoi(frame, hsvBlue, hsv0);

        // Старт тренировки
        //startTraining();

        // Старт распознавателя для красного цвета
        for(unsigned int i = 0; i < roiR.size(); i++){
            RD->startClassifier(roiR[i]);
        }
        // Старт распознавателя для синего цвета
        for(unsigned int i = 0; i < roiB.size(); i++){
            RD->startClassifier(roiB[i]);
        }
        if(waitKey(70) >= 0) break;
    }
}

// Starting SVM classifier
void roadSignsDetection::startClassifier(const Mat& img) {

    // Создание единственного объекта класса распознавателя
    static bool isSVMload = false;
    static signClassifier *SC;
    if(!isSVMload){
        SC = new signClassifier();
        isSVMload = true;
    }

    static vector<int> numbers;
    // Возвращение номера папки с рапознанным знаком
    int value = SC->detectDigit(img);

    // Выборка результатов : минимализация ошибок
    if(value != 6)
        numbers.push_back(value);
    if(numbers.size() == 10)
        for (int i = 0; i < 9; i++){
            if(numbers[i] == numbers[i + 1])
                continue;
            else{
                numbers.erase(numbers.begin(), numbers.begin() + i);
                break;
            }
        }

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(numbers.size() == 10){
        numbers.erase(numbers.begin(), numbers.end());
        switch (value) {
        case 0:
            qDebug() <<"Stop";
            break;
        case 1:
            qDebug() <<"Up";
            break;
        case 2:
            qDebug() <<"Right";
            break;
        case 3:
            qDebug() <<"Left";
            break;
        case 4:
            qDebug() <<"Up or right";
            break;
        case 5:
            qDebug() <<"Up or left";
            break;
        case 6:
            qDebug() <<"Nothing";
            break;
        }
    }
}

// Training SVM
void roadSignsDetection::startTraining() {
    // Объект класса тренировки по цифрам
    signTrainer *ST;
    ST = new signTrainer();

    // SVM training run
    ST->performTraining();
    delete ST;
}


vector<Mat> roadSignsDetection::startFindRoi(Mat& frame, roiFinder::hsvParams p1, roiFinder::hsvParams p2){
    roiFinder* RF;
    RF = new roiFinder();

    vector<Mat> r = RF->startSearch(frame, p1, p2);;
    delete RF;

    return r;
}
