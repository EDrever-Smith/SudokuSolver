#include "sudokugui.h"
#include "ui_sudokugui.h"

using namespace cv;
using namespace std;

SudokuGui::SudokuGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SudokuGui)
{
    ui->setupUi(this);
    generateLabels();
    //Static array holding input sudoku puzzle to be solved. 0s represent free spaces
    int inputSudoku[81]      =   {3,0,6, 5,0,8, 4,0,0,
                                  5,2,0, 0,0,0, 0,0,0,
                                  0,8,7, 0,0,0, 0,3,1,

                                  0,0,3, 0,1,0, 0,8,0,
                                  9,0,0, 8,6,3, 0,0,5,
                                  0,5,0, 0,9,0, 6,0,0,

                                  1,3,0, 0,0,0, 2,5,0,
                                  0,0,0, 0,0,0, 0,7,4,
                                  0,0,5, 2,0,6, 3,0,0};
    //board = SudokuBoard(inputSudoku);
    board = SudokuBoard();
    displaySudokuBoard(board);

    modeButtons = new QButtonGroup(this);
    modeButtons->addButton(ui->playRadioButton,0);
    modeButtons->addButton(ui->solveRadioButton,1);

    ui->solveButton->setEnabled(false);

    connect(modeButtons, SIGNAL(buttonClicked(int)), this, SLOT(handleModeSelect(int)));
    connect(ui->solveButton, SIGNAL(released()), this, SLOT(solveCurrentBoard()));
}

SudokuGui::~SudokuGui()
{
    delete ui;
}

void SudokuGui::generateLabels()
{
    QGridLayout *layout = ui->gridLayout0;
    QGridLayout *grid1 = ui->gridLayout1;
    QGridLayout *grid2 = ui->gridLayout2;
    QGridLayout *grid3 = ui->gridLayout3;
    QGridLayout *grid4 = ui->gridLayout4;
    QGridLayout *grid5 = ui->gridLayout5;
    QGridLayout *grid6 = ui->gridLayout6;
    QGridLayout *grid7 = ui->gridLayout7;
    QGridLayout *grid8 = ui->gridLayout8;
    QGridLayout *grid9 = ui->gridLayout9;
    lineEditsArray = new QLineEdit*[81];
    for(int id=0; id<81; id++)
    {
        lineEditsArray[id] = new QLineEdit(this);

        QSizePolicy lineEditSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        lineEditsArray[id]->setSizePolicy(lineEditSizePolicy);

        QFont font( "Arial", 18, QFont::Bold);
        lineEditsArray[id]->setText(tr(""));
        lineEditsArray[id]->setFont(font);
        lineEditsArray[id]->setStyleSheet("QLineEdit {"
        "border-style: solid;"
        "border-width: 1px;"
        "border-color: black; "
        "}");
        lineEditsArray[id]->setAlignment(Qt::AlignCenter);
        lineEditsArray[id]->setMaxLength(1);
        lineEditsArray[id]->setReadOnly(true);

        if(id % 9 < 3 && id/9 <3)
            grid1->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 3)
            grid2->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 3)
            grid3->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 6)
            grid4->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 6)
            grid5->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 6)
            grid6->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 3 && id/9 < 9)
            grid7->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 6 && id/9 < 9)
            grid8->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);
        else if (id % 9 < 9 && id/9 < 9)
            grid9->addWidget(lineEditsArray[id],(id/9)%3, (id%9)%3);

        connect(lineEditsArray[id], SIGNAL(textEdited(QString)), this, SLOT(updateNumberSquare()));
    }
}
void SudokuGui::updateNumberSquare()
{
    QObject* obj = sender(); //gets a pointer to the object which sent the signal
    for(int i = 0; i < 81; i++)
    {
        if(obj == lineEditsArray[i])
        {
            if(board.isLegal(i,lineEditsArray[i]->text().toInt()))
            {
                board[i].setVal(lineEditsArray[i]->text().toInt());
                std::cout<<board<<std::endl;
            }
            else lineEditsArray[i]->setText(tr("")); //Users desired input breaks rules
        }
    }
}
void SudokuGui::displaySudokuBoard(SudokuBoard &s)
{
    for(int i = 0; i < 81; i++)
    {
        if(!(s[i].getVal() == 0))
            lineEditsArray[i]->setText(tr("%1").arg(s[i].getVal()));
    }
    return;
}

void SudokuGui::solveCurrentBoard()
{
    //TODO check case that unsolvable
    board.SolveSudoku();
    displaySudokuBoard(board);
}

void SudokuGui::handleModeSelect(int modeSelected)
{
    switch (modeSelected)
    {
    case 0:
        ui->playRadioButton->setEnabled(false);
        ui->solveRadioButton->setEnabled(true);

        generateGameBoardButton = new QPushButton(this);
        generateGameBoardButton->setText(tr("Generate New Board"));
        ui->horizontalLayout0->addWidget(generateGameBoardButton);

        //remove all widgets generated by the solve case
        if(previousModeFlag == 1)
        {
            ui->horizontalLayout0->removeWidget(manualInputButton);
            ui->horizontalLayout0->removeWidget(videoInputButton);
            ui->horizontalLayout0->removeWidget(pictureInputButton);
            delete manualInputButton;
            delete videoInputButton;
            delete pictureInputButton;
        }
        //Play
        //Setup lives
        //Setup timer
        //Setup Generator/Manual input
        //Setup hint functionality
        break;
    case 1:
        ui->solveRadioButton->setEnabled(false);
        ui->playRadioButton->setEnabled(true);

        manualInputButton = new QPushButton(this);
        videoInputButton = new QPushButton(this);
        pictureInputButton = new QPushButton(this);

        manualInputButton->setText(tr("Manual Input"));
        videoInputButton->setText(tr("Video Input"));
        pictureInputButton->setText(tr("Picture Input"));

        ui->horizontalLayout0->addWidget(manualInputButton);
        ui->horizontalLayout0->addWidget(videoInputButton);
        ui->horizontalLayout0->addWidget(pictureInputButton);

        //remove all widgets generated by the play case
        if(previousModeFlag == 0)
        {
            ui->horizontalLayout0->removeWidget(generateGameBoardButton);
            delete generateGameBoardButton;
        }

        //connect buttons to handler functions
        connect(manualInputButton, SIGNAL(released()), this, SLOT(handleManualInputSelected()));
        connect(videoInputButton, SIGNAL(released()), this, SLOT(handleVideoInputSelected()));
        connect(pictureInputButton, SIGNAL(released()), this, SLOT(handlePictureInputSelected()));
        break;
    }
    previousModeFlag = modeSelected;
}

void SudokuGui::handleManualInputSelected()
{
    manualInputButton->setFlat(true);
    videoInputButton->setFlat(false);
    pictureInputButton->setFlat(false);

    clearUiAndBoard();

    //Makes all line edits writable
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(false);

    ui->solveButton->setEnabled(true);
}

void SudokuGui::handleVideoInputSelected()
{
    manualInputButton->setFlat(false);
    videoInputButton->setFlat(true);
    pictureInputButton->setFlat(false);

    clearUiAndBoard();

    //Makes all line edits read only
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(true);
}

void SudokuGui::handlePictureInputSelected()
{
    manualInputButton->setFlat(false);
    videoInputButton->setFlat(false);
    pictureInputButton->setFlat(true);

    clearUiAndBoard();

    //Makes all line edits read only
    for(int i = 0; i < 81; i++)
        lineEditsArray[i]->setReadOnly(true);

    // set the filter for STL file
    QString filter = "Images (*.png *.jpg)";
    // obtain the file name
    QString filename = QFileDialog::getOpenFileName(this, QString("Open STL file"), "./", filter);

    if (filename.isEmpty()) //if no user exits file dialog rest of function is skipped
    {
        pictureInputButton->setFlat(false);
        return;
    }

    Mat image = imread(filename.toStdString());
    // TODO Check for failure
    if (image.empty())
    {

    }
    //CV_8UC1 = 8-bit unsigned int
    cvtColor(image, image, COLOR_RGB2GRAY);
    Mat mainOutline = Mat(image.size(),  CV_8UC1);
    GaussianBlur(image, image, Size(11,11),0);
    adaptiveThreshold(image, mainOutline, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    bitwise_not(mainOutline, mainOutline);

    //Dilate the image to fill up lines -> removing any cracks
    Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
        dilate(mainOutline, mainOutline, kernel);


    //iterate through each pixel
    //floodfill
    //Keep track of bounding rect with largest area
    int max = -1;
    Point maxPoint;
    for(int y = 0; y < mainOutline.size().height; y++)
    {
        uchar *row = mainOutline.ptr(y);
        for(int x = 0; x < mainOutline.size().width; x++)
        {
            if(row[x] >= 128)
            {
                int boundingArea = floodFill(mainOutline, Point(x,y), CV_RGB(0,0,64));
                if(boundingArea > max)
                {
                    max = boundingArea;
                    maxPoint = Point(x,y);
                }
            }
        }
    }

    //Make the largest area white
    floodFill(mainOutline, maxPoint, CV_RGB(255,255,255));

    //flood all remaining grey pixels to be black leaving only the outline visible
    for(int y=0;y<mainOutline.size().height;y++)
    {
        uchar *row = mainOutline.ptr(y);
        for(int x=0;x<mainOutline.size().width;x++)
        {
            if(row[x]==64 && x!=maxPoint.x && y!=maxPoint.y)
            {
                int area = floodFill(mainOutline, Point(x,y), CV_RGB(0,0,0));
            }
        }
    }
    erode(mainOutline, mainOutline, kernel);

    //vector that will store lines in normal form (p, theta) as this is the output of the hough transform
    std::vector<Vec2f> lines;

    //performs the hough transform on the main outline https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html
    //args 2 and 3 are the resolution params for p and theta. arg 4 is the threshold. I.e minimum number of intersections required to detect line
    HoughLines(mainOutline, lines, 1, CV_PI/180, 200);

    findAndMergeCloseLines(&lines, mainOutline);



    for(int i=0;i<lines.size();i++)
        {
            drawLine(lines[i], mainOutline, CV_RGB(0,0,128));
        }



    String windowName = "SudokuImage"; //Name of the window

    namedWindow(windowName); // Create a window

    imshow(windowName, mainOutline); // Show our image inside the created window.

    waitKey(0); // Wait for any keystroke in the window

    destroyWindow(windowName); //destroy the created window

}

void SudokuGui::clearUiAndBoard()
{
    //Not using SudokuBoard's clearBoard function as would require two loops to clear both Ui and board data
    for(int i = 0; i < 81; i++)
    {
        this->board[i].setVal(0);
        lineEditsArray[i]->setText(tr(""));
    }
}

//Function to take lines in polar normal form and convert them to cartesian and draw them.
//y = (−cotθ)x+(p/sinθ)
//m = -cotθ
//c = p/sinθ
void drawLine(Vec2f polarLine, Mat &image, Scalar rgb)
{
    if(polarLine[1]!=0) //checks to see if line is vertical (theta = 0) if not, then line must have y-intercept
    {
        float m = -1/tan(polarLine[1]);
        float c = polarLine[0]/sin(polarLine[1]);

        line(image, Point(0, c), Point((image.size().width),(m*image.size().width+c)), rgb);
    }
    else //if line is vertical, no y-intercept but can easily draw it at x=r
    {
        line(image, Point(polarLine[0], 0), Point(polarLine[0], image.size().height), rgb);
    }

}

//Function to take a vector of lines and compute which lines are close together and average such lines
void findAndMergeCloseLines(vector<Vec2f> *lines, Mat &image)
{
    for(int currentLine = 0; currentLine < (*lines).size(); currentLine++)
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
        if(currentTheta > CV_PI*45/180 && currentTheta < CV_PI*135/180)
        {
            currentPoint1.x = 0;
            currentPoint1.y = currentRho / sin(currentTheta);

            currentPoint2.x = image.size().width;
            currentPoint2.y = -currentPoint2.x/tan(currentTheta) + currentRho / cos(currentTheta);
        }
        else
        {
            currentPoint1.y = 0;
            currentPoint1.x = currentRho / cos(currentTheta);

            currentPoint2.y = image.size().height;
            currentPoint2.x = currentPoint2.y * tan(currentTheta) + currentRho / cos(currentTheta);
        }
        //Compare each line to every other line
        for(int compLine = 0; compLine < (*lines).size(); compLine++)
        {
            if(currentLine = compLine) continue;

            float rhoThresh = 20;
            float thetaThresh = CV_PI*10/180;
            //Check whether lines are close to eachother (within rho and theta threshold)
            if(fabs((*lines)[currentLine][0] - (*lines)[compLine][0]) < rhoThresh && fabs((*lines)[currentLine][1] - (*lines)[compLine][1]) < thetaThresh)
            {
                float compRho = (*lines)[compLine][0];
                float compTheta = (*lines)[compLine][1];

                Point compPoint1, compPoint2;
                //Find two points on the extremes of the line we are comparing
                if(compTheta > CV_PI*45/180 && compTheta < CV_PI*135/180)
                {
                    compPoint1.x = 0;
                    compPoint1.y = compRho / sin(compTheta);

                    compPoint2.x = image.size().width;
                    compPoint2.y = -compPoint2.x/tan(compTheta) + compRho / cos(compTheta);
                }
                else
                {
                    compPoint1.y = 0;
                    compPoint1.x = compRho / cos(compTheta);

                    compPoint2.y = image.size().height;
                    compPoint2.x = compPoint2.y * tan(compTheta) + compRho / cos(compTheta);
                }

                //TODO Verify if this is or isn't overkill since we are already making a check to see if lines are close to eachother earlier
                float distanceThresh = 64*64;
                //if both pairs of points are within the distance threshold from eachother then they will be merged
                if(pow(currentPoint1.x - compPoint1.x,2) + pow(currentPoint1.y - compPoint1.y,2) < distanceThresh &&
                   pow(currentPoint2.x - compPoint2.x,2) + pow(currentPoint2.y - compPoint2.y,2) < distanceThresh)
                {
                    //Average the rho and delta values of the current and comparison lines
                    (*lines)[currentLine][0] = ((*lines)[currentLine][0] + (*lines)[compLine][0])/2;
                    (*lines)[currentLine][1] = ((*lines)[currentLine][1] + (*lines)[compLine][1])/2;

                    vector<Vec2f>::iterator it = lines->begin();
                    advance(it,compLine);
                    lines->erase(it);
                }
            }
        }
    }
}
