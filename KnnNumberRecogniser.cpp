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

bool KnnNumberRecogniser::readMNIST(char* imagesFilePath, char* labelsFilePath, Mat& imageData, Mat& labelData)
{
  FILE* imagesFileStream = fopen(imagesFilePath, "rb");
  FILE* labelsFileStream = fopen(labelsFilePath, "rb");

  //check if files opened successfully
  if(!imagesFileStream || !labelsFileStream)
    return false;

  //first 4 items in training set image file are 32 bit ints: magic number, number of images, number of rows, number of colums
  cout << "ImageFile MagicNumber: " << readFlippedInteger(imagesFileStream) << endl; //2051 for traindata
  cout << "LabelFile MagicNumber: " << readFlippedInteger(labelsFileStream) << endl; //2049 for trainlabels

  int32_t numImages = readFlippedInteger(imagesFileStream);
  int32_t numRows = readFlippedInteger(imagesFileStream);
  int32_t numCols = readFlippedInteger(imagesFileStream);
  int32_t size = numRows * numCols;

  imageData = Mat(numImages, size, CV_8UC1);
  labelData = Mat(numImages, 1, CV_8UC1);

  //already tested magic number and know number of items so can skip first two ints
  fseek(labelsFileStream, 8, SEEK_SET);

  //For each image, read off all those pixels into a row of the data matricies
  for(int i = 0; i < numImages; i++)
  {
    uint8_t tempLabel = 0;
    uint8_t* tempPixels = new uint8_t[size];

    fread(&tempPixels, sizeof(uint8_t), 1, imagesFileStream);
    Mat tempMat(1,size, CV_8UC1, &tempPixels);
    tempMat.row(0).copyTo(imageData.row(i)); //Had to do this because push_back is finicky as hell
    //delete array

    fread(&tempLabel,sizeof(uint8_t), 1, labelsFileStream);
    //cout << tempLabel << endl;
    labelData.at<uchar>(i, 0) = tempLabel;
  }
  fclose(imagesFileStream);
  fclose(labelsFileStream);

  return true;
}
bool KnnNumberRecogniser::train(char* trainImagesPath, char* trainLabelsPath)
{
  //Matricies to hold training data (row-wise)
  Mat trainingImages;
  Mat trainingLabels;

  if(!readMNIST(trainImagesPath, trainLabelsPath, trainingImages, trainingLabels))
    return false;

  trainingImages.convertTo(trainingImages, CV_32FC1); //Change from 8bit unsigned char to 32bit float ready for train function
  trainingLabels.convertTo(trainingLabels, CV_32FC1);
  knn->setDefaultK(10);
  knn->train(trainingImages, ml::ROW_SAMPLE, trainingLabels);

  return true;
}
float KnnNumberRecogniser::test(char* testDataPath, char* testLabelsPath)
{
  Mat testImages;
  Mat testLabels;

  readMNIST(testDataPath, testLabelsPath, testImages, testLabels);

  int correctCount = 0;

  testImages.convertTo(testImages, CV_32FC1);
  testLabels.convertTo(testLabels, CV_32FC1);

  for(int i = 0; i < testImages.rows; i++)
  {
    Mat results(1,testImages.cols,CV_32FC1);
    float response = knn->findNearest(testImages.row(i), knn->getDefaultK(), results);
    //cout << response << endl;
    if(testLabels.at<float>(i,0) == response)
      correctCount++;
  }
  return correctCount / testImages.rows;
}

Mat KnnNumberRecogniser::preprocessImages(Mat image)
{
  return image;
}

vector<int> KnnNumberRecogniser::identifyNumbers(Mat images)
{
  vector<int> results;
  results.resize(81);
  Mat resultMat;
  images.convertTo(images, CV_32FC1);
  knn->findNearest(images, knn->getDefaultK(), resultMat);
  //cout << resultMat << endl;
  return results;
}
