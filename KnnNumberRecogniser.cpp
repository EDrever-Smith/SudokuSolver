#include "KnnNumberRecogniser.h"

KnnNumberRecogniser::KnnNumberRecogniser()
{
    knn = ml::KNearest::create();
}

KnnNumberRecogniser::~KnnNumberRecogniser()
{
    delete knn;
}
//Function to read ints from MNIST data set (big-endian)
int32_t KnnNumberRecogniser::readFlippedInteger(FILE *fileStream)
{
  int32_t returnInt = 0;
  uint8_t* pReturnBytes;
  pReturnBytes = (uint8_t*)(&returnInt);
  fread(&pReturnBytes[3], sizeof(uint8_t), 1, fileStream);
  fread(&pReturnBytes[2], sizeof(uint8_t), 1, fileStream);
  fread(&pReturnBytes[1], sizeof(uint8_t), 1, fileStream);
  fread(&pReturnBytes[0], sizeof(uint8_t), 1, fileStream);

  return returnInt;
}

bool KnnNumberRecogniser::train(char* trainImagesPath, char* trainLabelsPath)
{
  FILE *trainImagesStream = fopen(trainImagesPath, "rb");
  FILE *trainLabelsStream = fopen(trainLabelsPath, "rb");

  //check if files opened successfully
  if(!trainImagesStream || !trainLabelsStream)
    return false;

  //first 4 items in training set image file are 32 bit ints: magic number, number of images, number of rows, number of colums
  if(readFlippedInteger(trainImagesStream) != 2051) //If magic number isnt 2051 we are reading data incorrectly
    return false;

  numImages = readFlippedInteger(trainImagesStream);
  numRows = readFlippedInteger(trainImagesStream);
  numCols = readFlippedInteger(trainImagesStream);

  //already tested magic number and know number of items so can skip first two ints
  fseek(trainLabelsStream, 8, SEEK_SET);
  int size = numRows * numCols;

  //Matricies to hold training data (row-wise)
  Mat trainingImages(numImages, size, CV_32FC1);
  Mat trainingLabels(numImages, 1, CV_32FC1);

  uint8_t tempLabel = 0;
  uint8_t* tempPixels = new uint8_t[size];

  //For each image, read off all those pixels into a row of the data matricies
  for(int i = 0; i < numImages; i++)
  {
    fread(&tempPixels, sizeof(uint8_t), 1, trainImagesStream);
    Mat tempMat(1,size, CV_32FC1, &tempPixels);
    tempMat.row(0).copyTo(trainingImages.row(i)); //Had to do this because push_back is finicky as hell

    fread(&tempLabel,sizeof(uint8_t), 1, trainLabelsStream);
    Mat tempLabelMat(1, 1, CV_32FC1, &tempLabel);
    tempLabelMat.row(0).copyTo(trainingLabels.row(i));
  }
  knn->train(trainingImages, ml::ROW_SAMPLE, trainingLabels);
}

Mat KnnNumberRecogniser::preprocessImage(Mat image)
{
  return image;
}

int KnnNumberRecogniser::identifyNumber(Mat image)
{
  return 0;
}
