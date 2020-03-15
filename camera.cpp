#include "camera.h"

Camera::Camera(QWidget *parent) : QMainWindow(parent)
{

}

Camera::Camera(const string & intrinsic_filename,const string & extrinsic_filename){

    cv::Size imageSize=cv::Size(imageWidth,imageHeight);

    FileStorage fs(intrinsic_filename, FileStorage::READ);
    if (!fs.isOpened()){
        printf("Failed to open file %s\n", intrinsic_filename.c_str());
    }
    fs["M1"] >> cameraMatrixL;
    fs["D1"] >> distCoeffL;
    fs["M2"] >> cameraMatrixR;
    fs["D2"] >> distCoeffR;

    fs.open(extrinsic_filename, FileStorage::READ);
    if (!fs.isOpened()){
        printf("Failed to open file %s\n", extrinsic_filename.c_str());
    }

    fs["R"] >> R;
    fs["T"] >> T;

    Mat Rl, Rr, Pl, Pr;
    cv::Rect validROIL;
    cv::Rect validROIR;

    stereoRectify(cameraMatrixL, distCoeffL, cameraMatrixR, distCoeffR, imageSize, R, T, Rl, Rr, Pl, Pr, Q, CALIB_ZERO_DISPARITY,0, imageSize, &validROIL, &validROIR);

    initUndistortRectifyMap(cameraMatrixL, distCoeffL, Rl, Pr, imageSize, CV_32FC1, mapLx, mapLy);
    initUndistortRectifyMap(cameraMatrixR, distCoeffR, Rr, Pr, imageSize, CV_32FC1, mapRx, mapRy);
}

Mat Camera::getMatQ(){
    return Q;
}

vector<Mat> Camera::RectifyImg(Mat &src){
    vector<Mat> RectifiedImg;
    Mat rgbImageL = src(Rect(640, 0, 640, 480));
    Mat rgbImageR = src(Rect(0, 0, 640, 480));
    Mat rectifyImageLRBG,rectifyImageRRBG;
    remap(rgbImageL, rectifyImageLRBG, mapLx, mapLy, INTER_LINEAR);
    remap(rgbImageR, rectifyImageRRBG, mapRx, mapRy, INTER_LINEAR);
    //imshow("left",rectifyImageLRBG);
    //imshow("right",rectifyImageRRBG);
    RectifiedImg.push_back(rectifyImageLRBG);
    RectifiedImg.push_back(rectifyImageRRBG);
    return RectifiedImg;
}
