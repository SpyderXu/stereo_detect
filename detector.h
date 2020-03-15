#ifndef DETECTOR_H
#define DETECTOR_H
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
using namespace std;
using namespace caffe;

class Detector
{
public:
    Detector();
    Detector(const string& model_file,
               const string& weights_file,
               const string& mean_file,
               const string& mean_value);

    std::vector<vector<float> > Detect(const cv::Mat& img);

private:
  void SetMean(const string& mean_file, const string& mean_value);

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);

 private:
  shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
};

#endif // DETECTOR_H
