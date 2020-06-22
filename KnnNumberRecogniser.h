#ifndef KNN_NUMBER_RECOGNISER_H_INCLUDED
#define KNN_NUMBER_RECOGNISER_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <stddef.h>

using namespace cv;
using namespace std;

class KnnNumberRecogniser
{
public:
  KnnNumberRecogniser();
  ~KnnNumberRecogniser();

  bool train(char* trainDataPath, char* labelsPath);
  float test(char* testDataPath, char* testLabelsPath);
  vector<int> identifyNumbers(Mat image);
private:
  Mat preprocessImages(Mat image);
  bool readMNIST(char* imagesFileStream, char* labelsFileStream, Mat& imageData, Mat& labelData);
  int32_t readFlippedInteger(FILE *filePath);
  Ptr<ml::KNearest> knn;
};

#endif //KNN_NUMBER_RECOGNISER_H_INCLUDED
