#include "signtrainer.h"

void signTrainer::getsTrainingFiles()
{

    //Открываем каталог, в котором содержатся тренировочные файлы
    tinydir_dir training_root_dir;
    tinydir_open(&training_root_dir, "/home/liza/Downloads/Signs/");

    //Проходим по директориям
    while (training_root_dir.has_next)
    {
        cout<<"Directory next"<<endl;
        //Получаем имя файла
        tinydir_file file;
        tinydir_readfile(&training_root_dir, &file);

        // if it is a directory
        if(file.is_dir){

            std::string numbersDirName = file.name;
            //Имя директории
            cout<<"Name "<<numbersDirName<<endl;

            //Фильтруем отдельные директории
            if(numbersDirName != "." && numbersDirName != ".." && numbersDirName != ".DS_Store"){

                cout << "open file" << std::endl;

                // atoi isn't supersafe but sufficient for this test
                int currentLabel = atoi(file.name);

                //Добавляем полный путь к директории, в которой содержатся файлы
                numbersDirName.insert(0, "/home/liza/Downloads/Signs/");

                // open directory containing number directories
                tinydir_dir training_number_dir;
                tinydir_open(&training_number_dir, numbersDirName.c_str() );

                int numberOfUsedFiles = 0;
                // iterate over directories
                while (training_number_dir.has_next)
                {
                    cout << "file" << std::endl;

                    // get file
                    tinydir_file trainingJpgFile;
                    tinydir_readfile(&training_number_dir, &trainingJpgFile);

                    // get directory name
                    std::string trainingJpgFileName = trainingJpgFile.name;

                    cout<<"Reading file "<<trainingJpgFileName<<endl;

                    //Имя отдельного файла
                    cout<<"File name "<< numbersDirName.c_str()<<"/"<<trainingJpgFileName<<endl;
                    cout<<"Number of training files "<<numberOfUsedFiles<<endl;
                    cout<<"Current label "<<currentLabel<<endl;

                    // skip . / .. / .DS_Store (OSX)
                    if (trainingJpgFileName != "." && trainingJpgFileName != ".." && trainingJpgFileName != ".DS_Store"){

                        // prepend full training_files directory
                        trainingJpgFileName.insert(0, numbersDirName + "/");

                        // store training filename and label
//                        Mat imgMat = imread(trainingJpgFileName, CV_32FC1);
//                        if(imgMat.rows == 226 && imgMat.cols == 226 ) {
                            trainingFilenames.push_back(trainingJpgFileName);
                            numberOfUsedFiles++;
                            labels.push_back(currentLabel);
//                        }
//                        else {
//                            cout<<"Wrong size: "<<trainingJpgFileName<<endl;
//                        }

                    }

                    // get next file
                    tinydir_next(&training_number_dir);
                }

                // close directory
                tinydir_close(&training_number_dir);
            }
        }

        // get next file
        tinydir_next(&training_root_dir);
    }

    // close directory
    tinydir_close(&training_root_dir);
}


signTrainer::signTrainer()
{
    trainingFilenames.clear();
    labels.clear();
}

void signTrainer::performTraining() {

    qDebug()<<"Digits trainer is starting ...";

    //Загружаем файлы тренировки
    this->getsTrainingFiles();

    //Загружаем размер изображения
    int imgArea = 35 * 35;

    //Формируем матрицу для тренировочных файлов
    cv::Mat trainingMat(trainingFilenames.size(), imgArea, CV_32FC1);

    //Проходим по всем файлам
    for(uint index = 0; index<trainingFilenames.size(); index++)
    {
        // output on which file we are training
        //            qDebug()<<"Analyzing label -> file: "<<  labels[index];// << "|" <<  trainingFilenames[index]*/;

        // read image file (grayscale)
        cout<<"Training files read "<<index<<" of "<<trainingFilenames.size()<<endl;
        Mat imgMat = imread(trainingFilenames[index], 0);
        Rect roi;
        roi.width = 35;
        roi.height = 35;
        resize( imgMat, imgMat, roi.size() );
//        imshow("Train", imgMat);
//        waitKey(10);
        int ii = 0; // Current column in training_mat
        // process individual pixels and add to our training material
        for (int i = 0; i<imgMat.rows; i++) {
            for (int j = 0; j < imgMat.cols; j++) {
                trainingMat.at<float>(index, ii++) = imgMat.at<uchar>(i,j);
            }
        }
    }

    //
    // process labels

    int labelsArray[labels.size()];

    // loop over labels
    for(uint index=0; index<labels.size(); index++)
    {
        labelsArray[index] = labels[index];
    }

    Mat labelsMat(labels.size(), 1, CV_32S, labelsArray);

    //
    // train SVM
    // Set up SVM's parameters
//    Cv params;
//    params.svmType    = SVM::C_SVC;
//    params.kernel_type = SVM::POLY;
//    params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
//    params.gamma = 3;
//    params.degree = 3;
    //    CvSVM SVM;
    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(SVM::POLY);
    svm->setGamma(3);
    svm->setDegree(0.01);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-6));
    // Train the SVM with given parameters
    Ptr<TrainData> td = TrainData::create(trainingMat, ROW_SAMPLE, labelsMat);
    cout << "Start training SVM classifier" << endl;
    svm->train(td);
    cout << "Saving SVM data" <<endl;
    svm->save("/home/liza/Downloads/Signs/classifier.yml");
    qDebug()<<"Training complete";
}

