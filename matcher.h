#ifndef MATCHER_H
#define MATCHER_H
#include <caffe/caffe.hpp>
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
using namespace caffe;
using namespace std;

class Matcher
{
public:
    Matcher();


    Matcher(const string & model_file,
            const string & weights_file,
            const string & mean_file,
            const string & mean_value);
    cv::Mat Match(cv::Mat &Limg,cv::Mat &Rimg);
    cv::Mat getColorDisp();
    private:
    void WrapInputLayer(std::vector<cv::Mat> * input_channels0,std::vector<cv::Mat> * input_channels1);
    void Preprocess(cv::Mat &img0,cv::Mat img1, std::vector<cv::Mat> *input_channels0,std::vector<cv::Mat> * input_channels1);

    private:
    shared_ptr<Net<float> > net_;
    cv::Size input_geometry_;
    int num_channels_;
    cv::Mat mean_;
    cv::Mat color_disp;
};

#endif // MATCHER_H
