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
  int identifyNumber(Mat image);
private:
  Mat preprocessImage(Mat image);
  int32_t readFlippedInteger(FILE *filePath);
  Ptr<ml::KNearest> knn;
  int32_t numRows, numCols, numImages;
};

#endif //KNN_NUMBER_RECOGNISER_H_INCLUDED
