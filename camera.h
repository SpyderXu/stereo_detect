#ifndef CAMERA_H
#define CAMERA_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core/utility.hpp"


#include <algorithm>
#include <iomanip>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <stdio.h>


#include<iostream>
#include<sstream>
#include<fstream>
#include<string>

using namespace cv;
using namespace std;

class Camera : public QMainWindow
{
    Q_OBJECT
public:
    explicit Camera(QWidget *parent = 0);
    Camera(const string & intrinsic_filename,const string &extrinsic_filename);
    vector<Mat> RectifyImg(Mat &src);
    Mat getMatQ();
private:
        // param
        const int imageWidth=640;
        const int imageHeight=480;
        Mat cameraMatrixL;
        Mat distCoeffL;
        Mat cameraMatrixR;
        Mat distCoeffR;
        Mat R;
        Mat T;
        Mat Q;
        Mat mapLx;
        Mat mapLy;
        Mat mapRx;
        Mat mapRy;

signals:

public slots:
};

#endif // CAMERA_H
