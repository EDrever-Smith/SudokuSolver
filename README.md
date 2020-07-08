# SudokuSolver - A sudoku puzzle solver using backtracking.
Dependencies: Qt, OpenCV
## Main Aims
+ Design a GUI that enables user to input their own sudoku [x]
+ Use computer vision to allow the program to solve sudokus through a camera in real time. []
## Additional Goals
+ Generate 'random' solvable sudokus or the user to solve []
    + Generate random completed sudoku [x]
    + Make squares blank whilst still leaving enough clues []
+ Gameify the program (Timer, Leaderboard etc.) []
+ Visualise the process of how the sudoku is solved []
### To-Do List
+ Preprocess each number square by deskewing and centering them ready to be classified
+ Compare the effectiveness of kNN and SVM approaches to training the digit classifier
+ Train chosen algorithm using MNIST dataset (big endian so must write a function to read flipped ints on my little endian machine)
+ Classify 81x1 array of image squares and input data to SudokuBoard class
+ Determine a method of identifying whether video frame contains a sudoku that is processor efficient
