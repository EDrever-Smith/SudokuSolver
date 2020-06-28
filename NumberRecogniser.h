#ifndef NUMBER_RECOGNISER_H_INCLUDED
#define NUMBER_RECOGNISER_H_INCLUDED

#include <opencv2/opencv.hpp>
#include <opencv2/ml.hpp>
#include <iostream>
#include <stddef.h>
#include <fstream>

using namespace cv;
using namespace std;

class NumberRecogniser
{
public:
  NumberRecogniser();
  ~NumberRecogniser();

  virtual bool train(const string imagesFilePath, const string labelsFilePath) = 0;
  virtual float test(const string imagesFilePath, const string labelsFilePath) = 0;
  virtual vector<int> identifyNumbers(Mat image) = 0;

protected:
  Mat preprocessImages(Mat image);
  bool readMNIST(const string imagesFilePath, const string labelsFilePath, Mat& imageData, Mat& labelData);
  int32_t readFlippedInteger(ifstream& filePath);
  vector<int> results;
};

#endif //NUMBER_RECOGNISER_H_INCLUDED
