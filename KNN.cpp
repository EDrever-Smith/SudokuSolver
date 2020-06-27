#include "KNN.h"

KNN::KNN()
{
	knn = ml::KNearest::create();
}

KNN::~KNN()
{
	delete knn;
}

bool KNN::train(const string imagesFilePath, const string labelsFilePath)
{
    //Matricies to hold training data (row-wise)
    Mat trainingImages;
    Mat trainingLabels;

    if (!readMNIST(imagesFilePath, labelsFilePath, trainingImages, trainingLabels))
        return false;

    trainingImages.convertTo(trainingImages, CV_32FC1); //Change from 8bit unsigned char to 32bit float ready for train function
    trainingLabels.convertTo(trainingLabels, CV_32FC1);
    knn->setDefaultK(10);
    knn->train(trainingImages, ml::ROW_SAMPLE, trainingLabels);

    return true;
}

float KNN::test(const string imagesFilePath, const string labelsFilePath)
{
    Mat testImages;
    Mat testLabels;

    readMNIST(imagesFilePath, labelsFilePath, testImages, testLabels);

    int correctCount = 0;

    testImages.convertTo(testImages, CV_32FC1);
    testLabels.convertTo(testLabels, CV_32FC1);

    for (int i = 0; i < testImages.rows; i++)
    {
        Mat results;
        float response = knn->findNearest(testImages.row(i), knn->getDefaultK(), results);
        if (testLabels.at<float>(i, 0) == response)
            correctCount++;
    }
    return correctCount / testImages.rows;
}

vector<int> KNN::identifyNumbers(Mat images)
{
    vector<int> results;
    results.resize(images.rows);
    Mat resultMat;
    images.convertTo(images, CV_32FC1);

    for (int i = 0; i < images.rows; i++)
    {
        results[i] = (int)knn->findNearest(images.row(i), knn->getDefaultK(), resultMat);
        cout << "Sudoku Square " << i << " prediction: " << results[i] << endl;
    }
    return results;
}
