#ifndef SIMPLEDEMO_H
#define SIMPLEDEMO_H
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
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;
class SimpleDemo
{
public:
    SimpleDemo();
    SimpleDemo(std::vector<Point3f> & points);
    string Int2String(int num);
    void showDemo();
    Mat getDemo();
private:
     Mat demoImg;
     vector<Point3f> D3points;
};

#endif // SIMPLEDEMO_H
