#ifndef SGBMMATCHING_H
#define SGBMMATCHING_H

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

class sgbmMatching
{
public:
    sgbmMatching();
    sgbmMatching(Mat & Q,int blockSize , int uniquenessRatio,int numDisparities,int mode );
    Mat getDisp(Mat & rectifiedImgL,Mat & rectifiedImgR);
    Mat getXYX(Mat & rectifiedImgL,Mat & rectifiedImgR);
    Point3f get3Dpos(Mat & rectifiedImgL,Mat & rectifiedImgR,Rect &pos);
    Point3f cluster(vector<Point3f>& D3points);
private:
        Mat Q;
        Ptr<StereoSGBM> sgbm = StereoSGBM::create(0, 16, 3);
        int blockSize;
        int uniquenessRatio;
        int numDisparities;
        int mode;
};

#endif // SGBMMATCHING_H
