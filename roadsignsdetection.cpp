#include "roadsignsdetection.h"

void roadSignsDetection::startSignsDetection(){

    VideoCapture cap(1); // open the default camera
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

        roiFinder::roi r = RD->startFindRoi(frame);

        // Старт тренировки
        //startTraining();

        // Старт распознавателя для красного цвета
        for(unsigned int i = 0; i < r.roiR.size(); i++){
            RD->startClassifier(r.roiR[i]);
        }
        // Старт распознавателя для синего цвета
        for(unsigned int i = 0; i < r.roiB.size(); i++){
            RD->startClassifier(r.roiB[i]);
        }
        if(waitKey(70) >= 0) break;
    }
}

// Starting SVM classifier
void roadSignsDetection::startClassifier(const Mat& img) {

    // Создание единственного объекта класса распознавателя
    bool isSVMload = false;
    signClassifier *SC;
    if(!isSVMload){
        SC = new signClassifier();
        isSVMload = true;
    }

    static vector<int> numbers;
    // Возвращение номера папки с рапознанным знаком
    int value = SC->detectDigit(img);

    if(value != 6)
        numbers.push_back(value);
    if(numbers.size() == 5)
        for(int i = 0; i < 4; i++){
            if(numbers[i] == numbers[i + 1])
                continue;
            else{
                numbers.erase(numbers.begin(), numbers.begin() + i);
                break;
            }
        }

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    if(numbers.size() == 5){
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


roiFinder::roi roadSignsDetection::startFindRoi(Mat& frame){
    roiFinder* RF;
    RF = new roiFinder;

    roiFinder::roi r = RF->startSearch(frame);;
    delete RF;

    return r;
}
