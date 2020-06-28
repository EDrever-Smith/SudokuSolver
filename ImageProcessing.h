#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

void findMainOutline(Mat& mainOutline);
void drawLine(Vec2f line, Mat& img, Scalar rgb = CV_RGB(0, 0, 255));
void findAndMergeCloseLines(vector<Vec2f>* lines, Mat& image);
void findEdges(const vector<Vec2f>& lines, vector<Vec2f>& edges);
bool findIntersectionPoint(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f& intersectionPoint);
void getEdgeIntersectionPoints(vector<Vec2f>& edges, Point2f intersectionPoints[4], Mat& image);
double getImageSideLength(Point2f intersectionPoints[4]);