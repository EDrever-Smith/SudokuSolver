#ifndef KNN_NUMBER_RECOGNISER_H_INCLUDED
#define KNN_NUMBER_RECOGNISER_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <stddef.h>
#include <fstream>

using namespace cv;
using namespace std;

class KnnNumberRecogniser
{
public:
  KnnNumberRecogniser();
  ~KnnNumberRecogniser();

  bool train(const string imagesFilePath, const string labelsFilePath);
  float test(const string imagesFilePath, const string labelsFilePath);
  vector<int> identifyNumbers(Mat image);
private:
  Mat preprocessImages(Mat image);
  bool readMNIST(const string imagesFilePath, const string labelsFilePath, Mat& imageData, Mat& labelData);
  int32_t readFlippedInteger(ifstream& filePath);
  Ptr<ml::KNearest> knn;
};

#endif //KNN_NUMBER_RECOGNISER_H_INCLUDED
