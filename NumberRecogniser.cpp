#include "NumberRecogniser.h"

NumberRecogniser::NumberRecogniser() {}

NumberRecogniser::~NumberRecogniser() {}

//Function to read ints from MNIST data set (big-endian)
int32_t NumberRecogniser::readFlippedInteger(ifstream& fileStream)
{
    uint8_t bytes[4];
    fileStream.read((char*)&bytes[0], sizeof(uint8_t));
    fileStream.read((char*)&bytes[1], sizeof(uint8_t));
    fileStream.read((char*)&bytes[2], sizeof(uint8_t));
    fileStream.read((char*)&bytes[3], sizeof(uint8_t));

    return (int32_t)((bytes[3] << 0) | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24));
}
bool NumberRecogniser::readMNIST(const string imagesFilePath, const string labelsFilePath, Mat& imageData, Mat& labelData)
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


Mat NumberRecogniser::preprocessImages(Mat image)
{
   return image;
}
