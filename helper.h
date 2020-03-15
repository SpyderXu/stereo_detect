#ifndef HELPER_H
#define HELPER_H

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
class Helper
{
public:
    Helper();
    Helper(Mat &disp,Rect & pos,Mat &Q);
    Point3f getD3pos();
    Point3f cluster(vector<Point3f>& D3points);
    Point3f postProcess(Point3f D3points);
private:
    Mat disp;
    cv::Rect pos;
    Mat Q;
};

#endif // HELPER_H

