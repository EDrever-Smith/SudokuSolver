#ifndef SUDOKUGUI_H
#define SUDOKUGUI_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

#include <QWidget>
#include <QLineEdit>
#include <QButtonGroup>
#include <QPushButton>
#include <QFileDialog>

#include <vector>

#include "SudokuBoard.h"
#include "NumberRecogniser.h"
#include "ImageProcessing.h"
#include "KNN.h"

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

#endif // SUDOKUGUI_H
