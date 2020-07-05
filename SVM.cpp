#include "SVM.h"

SVM::SVM()
{
	svm = ml::SVM::create();
	svm->setType(ml::SVM::C_SVC);
	svm->setKernel(ml::SVM::RBF);
	svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 500, 1e-6));
}

SVM::~SVM()
{
	 
}

bool SVM::train(const string imagesFilePath, const string labelsFilePath)
{
    //Matricies to hold training data (row-wise)
    Mat trainingImages;
    Mat trainingLabels;

    if (!readMNIST(imagesFilePath, labelsFilePath, trainingImages, trainingLabels))
        return false;

    trainingImages.convertTo(trainingImages, CV_32FC1); //Change from 8bit unsigned char to 32bit float ready for train function
    trainingLabels.convertTo(trainingLabels, CV_32SC1);
    
    svm->train(trainingImages, ml::ROW_SAMPLE, trainingLabels);
    svm->save("SVM_MNIST.xml"); //Takes a hell of a long time to train so probs need to train once then always retrieve the same model

    return true;
}

float SVM::test(const string imagesFilePath, const string labelsFilePath)
{
    Mat testImages;
    Mat testLabels;

    readMNIST(imagesFilePath, labelsFilePath, testImages, testLabels);

    int correctCount = 0;

    testImages.convertTo(testImages, CV_32FC1);
    testLabels.convertTo(testLabels, CV_32FC1);

    for (int i = 0; i < testImages.rows; i++)
    {
        float response = svm->predict(testImages.row(i));
        if (testLabels.at<float>(i, 0) == response)
            correctCount++;
    }
    return correctCount / testImages.rows;
}

vector<int> SVM::identifyNumbers(Mat image)
{
    Mat numberSquares = NumberRecogniser::preprocessImages(image);
    results.resize(numberSquares.rows);
    numberSquares.convertTo(numberSquares, CV_32FC1);

    for (int i = 0; i < numberSquares.rows; i++)
    {
        if (results[i] != 0) //If not blank square
        {
            results[i] = (int)svm->predict(numberSquares.row(i));
        }
        //cout << "Sudoku Square " << i << " prediction: " << results[i] << endl;
    }
    return results;
}