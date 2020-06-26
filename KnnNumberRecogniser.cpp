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
int32_t KnnNumberRecogniser::readFlippedInteger(ifstream& fileStream)
{
    uint8_t bytes[4];
    fileStream.read((char*)&bytes[0], sizeof(uint8_t));
    fileStream.read((char*)&bytes[1], sizeof(uint8_t));
    fileStream.read((char*)&bytes[2], sizeof(uint8_t));
    fileStream.read((char*)&bytes[3], sizeof(uint8_t));

    return (int32_t)((bytes[3] << 0) | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24));
}
bool KnnNumberRecogniser::readMNIST(const string imagesFilePath, const string labelsFilePath, Mat& imageData, Mat& labelData)
{
	ifstream imagesFileStream(imagesFilePath, ios::binary);
	ifstream labelsFileStream(labelsFilePath, ios::binary);

    //check if files opened successfully
    if (!(imagesFileStream.is_open() && labelsFileStream.is_open()))
        return false;

    //first 4 items in training set image file are 32 bit ints: magic number, number of images, number of rows, number of colums
    int32_t magicNumber = 0;
    magicNumber = readFlippedInteger(imagesFileStream);
    cout << "ImageFile MagicNumber: " << magicNumber << endl; //2051 for traindata
    magicNumber = readFlippedInteger(labelsFileStream);
    cout << "LabelFile MagicNumber: " << magicNumber << endl; //2049 for trainlabels

    int32_t numImages = readFlippedInteger(imagesFileStream);
    int32_t numRows = readFlippedInteger(imagesFileStream);
    int32_t numCols = readFlippedInteger(imagesFileStream);
    int32_t size = numRows * numCols;
    
    //already tested magic number and know number of items so can skip first two ints
    labelsFileStream.seekg(8);

    imageData = Mat(numImages, size, CV_8U);
    labelData = Mat(numImages, 1, CV_8U);

    //For each image, read off all those pixels into a row of the data matricies
	for (int i = 0; i < numImages; ++i)
    {
		uint8_t * pixelData = new uint8_t[size];
        imagesFileStream.read((char*)pixelData, sizeof(uint8_t) * size);
		Mat tempMat(1, size, CV_8U, pixelData);
		tempMat.row(0).copyTo(imageData.row(i)); //Had to do this because push_back is finicky as hell

		uint8_t label = 0;
        labelsFileStream.read((char*)&label, sizeof(label));
        labelData.at<uchar>(i, 0) = label;
    }
    imagesFileStream.close();
    labelsFileStream.close();
    return true;
}

bool KnnNumberRecogniser::train(const string imagesFilePath, const string labelsFilePath)
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
float KnnNumberRecogniser::test(const string imagesFilePath, const string labelsFilePath)
{
  Mat testImages;
  Mat testLabels;

  readMNIST(imagesFilePath, labelsFilePath, testImages, testLabels);

  int correctCount = 0;

  testImages.convertTo(testImages, CV_32FC1);
  testLabels.convertTo(testLabels, CV_32FC1);

  for(int i = 0; i < testImages.rows; i++)
  {
    Mat results;
    float response = knn->findNearest(testImages.row(i), knn->getDefaultK(), results);
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
    results.resize(images.rows);
    Mat resultMat;
    images.convertTo(images, CV_32FC1);

    for (int i = 0; i < images.rows; i++)
    {
        results[i] = (int)knn->findNearest(images, knn->getDefaultK(), resultMat);
        cout << "Image " << i << ": " << results[i] << endl;
    }
    return results;
}