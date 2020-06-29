#include "NumberRecogniser.h"

NumberRecogniser::NumberRecogniser() 
{
    results.resize(81);
    for (int i = 0; i < 81; i++)
        results[i] = -1;
}

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

//Function that takes image that has been perspective transformed and isolates each square and returns their pixel data on each row of output mat
Mat NumberRecogniser::preprocessImages(Mat image)
{
    //We use adaptive thresholding since the lighting can vary throughout the image.
    //This algorithm chooses the threshold for each pixel  depending on a small region surrounding said pixel
    //https://docs.opencv.org/3.4/d7/d4d/tutorial_py_thresholding.html
    adaptiveThreshold(image, image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 101, 1);

    cv::resize(image, image, Size(252, 252), 0, 0, INTER_LINEAR);
    //For loops iterate through image and takes each number square and 'unwraps' it to be stored as one row in the number squares mat
    Mat numberSquares(81, 28 * 28, CV_8U); //store images row-wise by pixel data
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            Mat tempMat(28 * 28, 1, CV_8U);
            for (int j = 0; j < 28; j++)
            {
                for (int i = 0; i < 28; i++)
                {
                    tempMat.at<uchar>(i + j * 28) = image.at<uchar>(j + y * 28, i + x * 28);
                }
            }
            tempMat = tempMat.t();
            tempMat.row(0).copyTo(numberSquares.row(x + y * 9));
        }
    }
    for (int i = 0; i < 81; i++)
    {
        /* Attempt to remove outer lines, in some case outerline intersected number and so number was also erased.
        Mat tempSquare(28, 28, CV_8U);
        tempSquare = numberSquares.row(i).reshape(1, 28);
        for (int j = 0; j < tempSquare.rows; j++)
        {
            floodFill(tempSquare, Point(0, j), Scalar(0, 0, 0));
            floodFill(tempSquare, Point(j, 0), Scalar(0, 0, 0));
            floodFill(tempSquare, Point(tempSquare.cols - 1, j), Scalar(0, 0, 0));
            floodFill(tempSquare, Point(j, tempSquare.rows - 1), Scalar(0, 0, 0));
        }
        imshow("Image", tempSquare);
        waitKey(0);
        */

        //Determine which squares are blank ... Pixel sums are too close between squares containing numbers and those without. Try looking at inner section to determine.
        /*int sumPixelValues = 0;
        for (int j = 0; j < numberSquares.cols; j++)
        {
            sumPixelValues += (int)numberSquares.at<uchar>(i, j);
        }
        cout << "Square " << i << " sum: " << sumPixelValues << endl;
        */

        Mat tempSquare(28, 28, CV_8U);
        tempSquare = numberSquares.row(i).reshape(1, 28);
        //imshow("Image", tempSquare);
        //waitKey(0);
        int innerSquareLength = 10;
        int sumPixelValues = 0;
        for (int j = (28 - innerSquareLength) / 2; j < (28 + innerSquareLength) / 2; j++)
        {
            for (int k = (28 - innerSquareLength) / 2; k < (28 + innerSquareLength) / 2; k++)
            {
                sumPixelValues += (int)tempSquare.at<uchar>(j, k);
            }
        }
        if (sumPixelValues < 5000) //blank square detected
            results[i] = 0;

    }
   return numberSquares;
}
