#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <opencv2/opencv.hpp>

#include <QWidget>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>
#include <QFileDialog>
#include <vector>

#include "SudokuBoard.h"

using namespace cv;
using namespace std;
namespace Ui {
class SudokuGui;
}

class SudokuGui : public QWidget
{
    Q_OBJECT

public:
    explicit SudokuGui(QWidget *parent = nullptr);
    ~SudokuGui();
    void displaySudokuBoard(SudokuBoard& s);
private slots:
    void solveCurrentBoard();
    void updateNumberSquare();
    void handleModeSelect(int modeSelected);
    void handleManualInputSelected();
    void handleVideoInputSelected();
    void handlePictureInputSelected();
    void clearUiAndBoard();

private:
    Ui::SudokuGui *ui;
    void generateLabels();
    QLineEdit** lineEditsArray;
    SudokuBoard board;
    QButtonGroup* modeButtons;
    QPushButton* manualInputButton;
    QPushButton* videoInputButton;
    QPushButton* pictureInputButton;
    QPushButton* generateGameBoardButton;

    int previousModeFlag = -1;
};

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255));
void findAndMergeCloseLines(vector<Vec2f> *lines, Mat &image);
void findEdges(const vector<Vec2f> &lines, vector<Vec2f> &edges);
bool findIntersectionPoint(Point2f o1, Point2f p1, Point2f o2, Point2f p2, Point2f &intersectionPoint);
void getEdgeIntersectionPoints(vector<Vec2f> &edges, Point2f intersectionPoints[4], Mat &image);

#endif // SUDOKUGUI_H

