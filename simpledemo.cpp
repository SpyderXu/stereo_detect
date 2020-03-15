#include "simpledemo.h"

SimpleDemo::SimpleDemo()
{

}

SimpleDemo::SimpleDemo(std::vector<Point3f> &points){
    demoImg=cv::imread("/home/cvpr/xcz/stereoMatching/UI/stereo/dst2.png");
    for(int i=0;i<points.size();i++){
        D3points.push_back(points[i]);
    }
}

void SimpleDemo::showDemo(){
    for(int i=0;i<D3points.size();i++){
        int x=D3points[i].x;
        int y=D3points[i].y;
        int z=D3points[i].z;
        string outputText=SimpleDemo::Int2String(int(x))+","+SimpleDemo::Int2String(int(y))+","+SimpleDemo::Int2String(int(z));
        cv::circle(demoImg,Point(int(x+500)/4,int(500-z*8)),8,CV_RGB(0,0,0),2);
        cv::putText(demoImg, outputText, Point(int(x+500)/4,int(500-z*8)), 0, 1, Scalar(0,0,0),1);
    }
    line(demoImg,Point(0,450),Point(500,450),Scalar(0,0,255),2);
    line(demoImg,Point(250,0),Point(250,500),Scalar(0,0,255),2);
}
Mat SimpleDemo::getDemo(){
    return demoImg;
}
string SimpleDemo::Int2String(int num){
    ostringstream stream;
    stream<<num;
    return stream.str();
}


