#include "ImageProcessing.h"

using namespace cv;
using namespace std;


void findMainOutline(Mat& mainOutline)
{
    //Dilate the image to fill up lines -> removing any cracks
    Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
    dilate(mainOutline, mainOutline, kernel);
    //imshow("Dilated", mainOutline);

    //iterate through each pixel
    //floodfill
    //Keep track of bounding rect with largest area
    int max = -1;
    Point maxPoint;
    for (int y = 0; y < mainOutline.size().height; y++)
    {
        uchar* row = mainOutline.ptr(y);
        for (int x = 0; x < mainOutline.size().width; x++)
        {
            if (row[x] >= 128)
            {
                int boundingArea = floodFill(mainOutline, Point(x, y), CV_RGB(0, 0, 64));
                if (boundingArea > max)
                {
                    max = boundingArea;
                    maxPoint = Point(x, y);
                }
            }
        }
    }

    //Make the largest area white
    floodFill(mainOutline, maxPoint, CV_RGB(255, 255, 255));

    //flood all remaining grey pixels to be black leaving only the outline visible
    for (int y = 0; y < mainOutline.size().height; y++)
    {
        uchar* row = mainOutline.ptr(y);
        for (int x = 0; x < mainOutline.size().width; x++)
        {
            if (row[x] == 64 && x != maxPoint.x && y != maxPoint.y)
            {
                int area = floodFill(mainOutline, Point(x, y), CV_RGB(0, 0, 0));
            }
        }
    }

    //undo dilate now that we have the mainOutline
    erode(mainOutline, mainOutline, kernel);
    //imshow("Dilated", mainOutline);
}
//Function to take lines in polar normal form and convert them to cartesian and draw them.
//y = (−cotθ)x+(p/sinθ).
//m = -cotθ
//c = p/sinθ
void drawLine(Vec2f polarLine, Mat& image, Scalar rgb)
{
    if (polarLine[1] != 0) //checks to see if line is vertical (theta = 0) if not, then line must have y-intercept
    {
        float m = -1 / tan(polarLine[1]);
        float c = polarLine[0] / sin(polarLine[1]);

        line(image, Point(0, c), Point((image.size().width), (m * image.size().width + c)), rgb);
    }
    else //if line is vertical, no y-intercept but can easily draw it at x=r
    {
        line(image, Point(polarLine[0], 0), Point(polarLine[0], image.size().height), rgb);
    }

}

//Function to take a vector of lines and compute which lines are close together and average such lines
void findAndMergeCloseLines(vector<Vec2f>* lines, Mat& image)
{
    for (int currentLine = 0; currentLine < (*lines).size(); currentLine++)
    {
        //Check if line has already been merged (will give merged lines these impossible values)
        //if((*lines)[currentLine][0] == 0 && (*lines)[currentLine][1] == -100) continue;

        float currentRho = (*lines)[currentLine][0];
        float currentTheta = (*lines)[currentLine][1];

        //using p and theta values for current line, will find two points on the line at extremes of the image that we can later use by taking their average to merge lines
        Point currentPoint1, currentPoint2;
        //So to avoid extremely large x or y values for the points, if the line is more horizontal than vertical (45<theta<135) the points will be found at the x extremes (x = 0 and image.width)
        //and if the line is more vertical the points will be found at the y extremes (y = 0 and image.height)
        //m = -cotθ
        //c = p/sinθ
        //y = mx + c
        //y = (−cotθ)x+(p/sinθ)
        //x = (y-c)/m
        //x = (y-p/sinθ)/-cotθ
        //  = (y*sinθ - p)/-cosθ
        //  = -y*tanθ + p/cosθ
        if (currentTheta > CV_PI * 45 / 180 && currentTheta < CV_PI * 135 / 180)
        {
            currentPoint1.x = 0;
            currentPoint1.y = currentRho / sin(currentTheta);

            currentPoint2.x = image.size().width;
            currentPoint2.y = -currentPoint2.x / tan(currentTheta) + currentRho / cos(currentTheta);
        }
        else
        {
            currentPoint1.y = 0;
            currentPoint1.x = currentRho / cos(currentTheta);

            currentPoint2.y = image.size().height;
            currentPoint2.x = -currentPoint2.y * tan(currentTheta) + currentRho / cos(currentTheta);
        }
        //Compare each line to every other line
        for (int compLine = 0; compLine < (*lines).size(); compLine++)
        {
            if (currentLine = compLine) continue;

            float rhoThresh = 20;
            float thetaThresh = CV_PI * 10 / 180;
            //Check whether lines are close to eachother (within rho and theta threshold)
            if (fabs((*lines)[currentLine][0] - (*lines)[compLine][0]) < rhoThresh && fabs((*lines)[currentLine][1] - (*lines)[compLine][1]) < thetaThresh)
            {
                float compRho = (*lines)[compLine][0];
                float compTheta = (*lines)[compLine][1];

                Point compPoint1, compPoint2;
                //Find two points on the extremes of the line we are comparing
                if (compTheta > CV_PI * 45 / 180 && compTheta < CV_PI * 135 / 180)
                {
                    compPoint1.x = 0;
                    compPoint1.y = compRho / sin(compTheta);

                    compPoint2.x = image.size().width;
                    compPoint2.y = -compPoint2.x / tan(compTheta) + compRho / cos(compTheta);
                }
                else
                {
                    compPoint1.y = 0;
                    compPoint1.x = compRho / cos(compTheta);

                    compPoint2.y = image.size().height;
                    compPoint2.x = -compPoint2.y * tan(compTheta) + compRho / cos(compTheta);
                }

                //TODO Verify if this is or isn't overkill since we are already making a check to see if lines are close to eachother earlier
                float distanceThresh = 64 * 64;
                //if both pairs of points are within the distance threshold from eachother then they will be merged
                if (pow(currentPoint1.x - compPoint1.x, 2) + pow(currentPoint1.y - compPoint1.y, 2) < distanceThresh &&
                    pow(currentPoint2.x - compPoint2.x, 2) + pow(currentPoint2.y - compPoint2.y, 2) < distanceThresh)
                {
                    //Average the rho and delta values of the current and comparison lines
                    (*lines)[currentLine][0] = ((*lines)[currentLine][0] + (*lines)[compLine][0]) / 2;
                    (*lines)[currentLine][1] = ((*lines)[currentLine][1] + (*lines)[compLine][1]) / 2;

                    vector<Vec2f>::iterator it = lines->begin();
                    advance(it, compLine);
                    lines->erase(it);
                }
            }
        }
    }
}

//Function to find the 4 most extreme lines in an array of lines
//Return array index content is as follows: {top,bottom,left,righ}
void findEdges(const vector<Vec2f>& lines, vector<Vec2f>& edges)
{
    Vec2f topEdge = Vec2f(DBL_MAX, 0);
    Vec2f bottomEdge = Vec2f(-DBL_MAX, 0);
    Vec2f leftEdge, rightEdge;

    double leftXIntercept = DBL_MAX, rightXIntercept = 0;

    for (int i = 0; i < lines.size(); i++)
    {
        Vec2f currentLine = lines[i];
        float currentRho = currentLine[0], currentTheta = currentLine[1];

        double xIntercept = currentRho / cos(currentTheta);
        double yIntercept = currentRho / (cos(currentTheta) * sin(currentTheta));

        if (currentTheta > CV_PI * 80 / 180 && currentTheta < CV_PI * 100 / 180)
        {
            if (currentRho < topEdge[0])
                topEdge = currentLine;

            if (currentRho > bottomEdge[0])
                bottomEdge = currentLine;
        }
        else if (currentTheta < CV_PI * 10 / 180 || currentTheta > CV_PI * 170 / 180)
        {
            if (xIntercept > rightXIntercept)
            {
                rightEdge = currentLine;
                rightXIntercept = xIntercept;
            }
            else if (xIntercept <= leftXIntercept)
            {
                leftEdge = currentLine;
                leftXIntercept = xIntercept;
            }
        }
    }
    edges.push_back(topEdge);
    edges.push_back(bottomEdge);
    edges.push_back(leftEdge);
    edges.push_back(rightEdge);
}

//https://stackoverflow.com/questions/7446126/opencv-2d-line-intersection-helper-function/7448287#7448287
bool findIntersectionPoint(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f& intersectionPoint)
{
    Point2f x = o2 - o1;
    Point2f d1 = p1 - o1;
    Point2f d2 = p2 - o2;

    float cross = d1.x * d2.y - d1.y * d2.x;
    if (abs(cross) < /*EPS*/1e-8)
        return false;

    double t1 = (x.x * d2.y - x.y * d2.x) / cross;
    intersectionPoint = o1 + d1 * t1;
    return true;
}

//Funtion to get the 4 intersection points of the passed edges
void getEdgeIntersectionPoints(vector<Vec2f>& edges, Point2f intersectionPoints[4], Mat& image)
{
    //for legibility
    Vec2f topEdge = edges[0], bottomEdge = edges[1], leftEdge = edges[2], rightEdge = edges[3];

    /* Debugging
    drawLine(topEdge, image, CV_RGB(0,0,0));
    drawLine(bottomEdge, image, CV_RGB(0,0,0));
    drawLine(leftEdge, image, CV_RGB(0,0,0));
    drawLine(rightEdge, image, CV_RGB(0,0,0));
    */

    //for legibility
    int height = image.size().height;
    int width = image.size().width;

    //Now find two points on each line with which to compute the lines intersection points
    Point2f top1, top2, bottom1, bottom2, left1, left2, right1, right2;

    left1.y = 0;
    left1.x = leftEdge[0] / cos(leftEdge[1]);
    left2.y = height;
    left2.x = left1.x - height * tan(leftEdge[1]);

    right1.y = 0;
    right1.x = rightEdge[0] / cos(rightEdge[1]);
    right2.y = height;
    right2.x = right1.x - height * tan(rightEdge[1]);

    bottom1.x = 0;
    bottom1.y = bottomEdge[0] / sin(bottomEdge[1]);
    bottom2.x = width;
    bottom2.y = -bottom2.x / tan(bottomEdge[1]) + bottom1.y;

    top1.x = 0;
    top1.y = topEdge[0] / sin(topEdge[1]);
    top2.x = width;
    top2.y = -top2.x / tan(topEdge[1]) + top1.y;

    //TODO catch failure of function
    findIntersectionPoint(top1, top2, left1, left2, intersectionPoints[0]);
    findIntersectionPoint(top1, top2, right1, right2, intersectionPoints[1]);
    findIntersectionPoint(bottom1, bottom2, left1, left2, intersectionPoints[2]);
    findIntersectionPoint(bottom1, bottom2, right1, right2, intersectionPoints[3]);
}

double getImageSideLength(Point2f intersectionPoints[4])
{
    //For legibility
    Point ptTopLeft = intersectionPoints[0], ptTopRight = intersectionPoints[1], ptBottomLeft = intersectionPoints[2], ptBottomRight = intersectionPoints[3];

    //Find the longest side length
    double imageSideLength = norm(ptTopLeft - ptTopRight);
    double temp = norm(ptTopLeft - ptBottomLeft);
    if (temp > imageSideLength)
        imageSideLength = temp;
    temp = norm(ptTopRight - ptBottomRight);
    if (temp > imageSideLength)
        imageSideLength = temp;
    temp = norm(ptBottomLeft - ptBottomRight);
    if (temp > imageSideLength)
        imageSideLength = temp;

    return imageSideLength;
}