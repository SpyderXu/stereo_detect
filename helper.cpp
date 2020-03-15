#include "helper.h"

Helper::Helper()
{

}

Helper::Helper(Mat &disp,Rect & pos,Mat &Q){
    this->disp=disp;
    this->Q=Q;
    this->pos.x = pos.x < 0 ? 0 : pos.x;
    this->pos.y = pos.y < 0 ? 0 : pos.y;
    this->pos.width = pos.width+pos.x < 640 ? pos.width : 640-pos.x;
    this->pos.height = pos.height+pos.y < 480 ? pos.height : 480-pos.y;
}
Point3f Helper::cluster(vector<Point3f>& D3points){
  if(D3points.size()==0){
    return Point3f(0.0,0.0,0.0);
  }
  float tempX=0.0;
  float tempY=0.0;
  float tempZ=0.0;
  Point3f XYZ;
  float depth=D3points[0].z;
  vector<float> depthVec;
  depthVec.push_back(depth);
  for(int i=1;i<D3points.size();i++){
    int flag=0;
    for(int j=0;j<depthVec.size();j++){
      if(fabs(D3points[i].z-depthVec[j])>20){
        flag++;
      }
    }
    if(flag==depthVec.size()){
      depthVec.push_back(D3points[i].z);
    }
  }
  vector<int> number;
  for (int j=0;j<depthVec.size();j++){
    number.push_back(0);
  }

  for(int i=0;i<D3points.size();i++){
    for (int j=0;j<depthVec.size();j++){
      if(fabs(D3points[i].z-depthVec[j])<=10){
        number[j]=number[j]+1;
      }
    }
  }
  int max=number[0];
  int pos=0;
  for(int i=0;i<number.size();i++){
    if(number[i]>max){
      max=number[i];
      pos=i;
    }
  }
  //cout<<"number[pos]:"<<number[pos]<<endl;
  int k=0;
  for(int i=0;i<D3points.size();i++){
    if(fabs(D3points[i].z-depthVec[pos])<=10){
      tempX=tempX+D3points[i].x;
      tempY=tempY+D3points[i].y;
      tempZ=tempZ+D3points[i].z;
      k++;
    }
  }
  tempX=tempX/float(k);
  tempY=tempY/float(k);
  tempZ=tempZ/float(k);
  XYZ.x=tempX;
  XYZ.y=tempY;
  XYZ.z=tempZ;
  return XYZ;
}

Point3f Helper::getD3pos(){
    Mat xyz;
    reprojectImageTo3D(disp, xyz, Q, true);
    const double max_z = 6000;
    Point3f returnXYZ;
    double x = 0.0;
    double y = 0.0;
    double depth=0.0;
    //cout << "pos:" << pos.x << " " << pos.y << " " << pos.width << " " << pos.height << endl;

    vector<Point3f> tempV;
    //using the rectangle in center to calculate the 3D pos
    for (int i = pos.y+pos.height*2/5; i < pos.y+pos.height*3/5; i++) {
       for (int j = pos.x+pos.width*2/5; j <pos.x+ pos.width*3/5; j++) {
         if (fabs(xyz.at<Vec3f>(i, j)[2] - max_z) < FLT_EPSILON || fabs(xyz.at<Vec3f>(i, j)[2]) > max_z) continue;
         else {
            Point3f temp;
            temp.x=xyz.at<Vec3f>(i, j)[0];
            temp.y=xyz.at<Vec3f>(i, j)[1];
            temp.z=xyz.at<Vec3f>(i, j)[2];
            tempV.push_back(temp);
      }
    }
  }
  Point3f clusterPos=Helper::cluster(tempV);
  tempV.clear();
  return clusterPos;
}
Point3f Helper::postProcess(Point3f D3points){
    D3points.z=0.01*31.0/29.0*D3points.z;
    return D3points;
}


