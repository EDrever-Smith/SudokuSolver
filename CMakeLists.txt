cmake_minimum_required(VERSION 3.12.0)

project(SudokuSolver)

set(CMAKE_PREFIX_PATH D:\\Qt\\5.12.0\\msvc2017_64;D:\\opencv\\build)
# The CMake build process might generate some new files in the current
# directory. This makes sure they can be found.
set( CMAKE_INCLUDE_CURRENT_DIR ON )

# This allows CMake to run one of Qt's build tools called moc
# if it is needed. moc.exe can be found in Qt's bin directory.
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

# Find the Qt widgets package. This locates the relevant include and
# lib directories, and the necessary static libraries for linking.
find_package( Qt5Widgets REQUIRED )

find_package( OpenCV REQUIRED )

set( UIS mainwindow.ui )
qt5_wrap_ui( UI_Srcs ${UIS} )

# TODO Sort out directory and make libraries
add_executable(SudokuApp main.cpp ${UI_Srcs} ${QRC_Srcs}
	NumberSquare.h NumberSquare.cpp
	SudokuBoard.h SudokuBoard.cpp
	sudokugui.h sudokugui.cpp sudokugui.ui
	NumberRecogniser.h NumberRecogniser.cpp
	ImageProcessing.h ImageProcessing.cpp
	KNN.h KNN.cpp SVM.h SVM.cpp)

# Tell CMake that the executable depends on the Qt::Widget libraries.
target_link_libraries( SudokuApp Qt5::Widgets ${OpenCV_LIBS})
