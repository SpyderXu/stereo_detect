#include "sgbmmatching.h"

sgbmMatching::sgbmMatching()
{

}

Point3f sgbmMatching::cluster(vector<Point3f>& D3points){
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
  //cout<<"size:"<<depthVec.size()<<endl;
  /*
  for(int l=0;l<depthVec.size();l++){
    cout<<"depth_kind:"<<depthVec[l]<<endl;
  }
  */
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

sgbmMatching::sgbmMatching(Mat & Q,int blockSize, int uniquenessRatio,int numDisparities ,int mode ){
    this->Q=Q;
    this->blockSize=blockSize;
    this->uniquenessRatio=uniquenessRatio;
    this->numDisparities=numDisparities;
    this->mode=mode;
    sgbm->setPreFilterCap(63);
    int sgbmWinSize = blockSize + 1;
    sgbm->setBlockSize(sgbmWinSize);

    int cn = 3;

    sgbm->setP1(8 * cn*sgbmWinSize*sgbmWinSize);
    sgbm->setP2(32 * cn*sgbmWinSize*sgbmWinSize);
    sgbm->setMinDisparity(0);
    sgbm->setNumDisparities(numDisparities * 16 + 16);
    sgbm->setUniquenessRatio(uniquenessRatio);
    sgbm->setSpeckleWindowSize(100);
    sgbm->setSpeckleRange(32);
    sgbm->setDisp12MaxDiff(1);
    if (mode == 0)
        sgbm->setMode(StereoSGBM::MODE_HH);
    else if (mode == 1)
        sgbm->setMode(StereoSGBM::MODE_SGBM);
    else if (mode == 2)
        sgbm->setMode(StereoSGBM::MODE_SGBM_3WAY);
}

Mat sgbmMatching::getDisp(Mat & rectifiedImgL,Mat & rectifiedImgR){
    Mat disp, disp8;
    sgbm->compute(rectifiedImgL, rectifiedImgR, disp);
    disp.convertTo(disp8, CV_8U, 255 / ((numDisparities * 16 + 16)*16.));
    return disp8;
}
Mat sgbmMatching::getXYX(Mat & rectifiedImgL,Mat & rectifiedImgR){
    Mat disp;
    Mat xyz;
    sgbm->compute(rectifiedImgL, rectifiedImgR, disp);
    reprojectImageTo3D(disp, xyz, Q, true);
    xyz = xyz * 16;
    return xyz;
}
Point3f sgbmMatching::get3Dpos(Mat & rectifiedImgL,Mat & rectifiedImgR,Rect &pos){
    Mat disp;
    Mat xyz;
    sgbm->compute(rectifiedImgL, rectifiedImgR, disp);
    reprojectImageTo3D(disp, xyz, Q, true);
    xyz = xyz * 16;

    const double max_z = 7000;
    Point3f returnXYZ;
    double x = 0.0;
    double y = 0.0;
    double depth=0.0;

    pos.x = pos.x < 0 ? 0 : pos.x;
    pos.y = pos.y < 0 ? 0 : pos.y;
    pos.width = pos.width+pos.x < 640 ? pos.width : 640-pos.x;
    pos.height = pos.height+pos.y < 480 ? pos.height : 480-pos.y;
    cout << "pos:" << pos.x << " " << pos.y << " " << pos.width << " " << pos.height << endl;
    vector<Point3f> tempV;
    //using the rectangle in center to calculate the 3D pos
    for (int i = pos.y+pos.height*1/3; i < pos.y+pos.height*2/3; i++) {
        for (int j = pos.x+pos.width*1/3; j <pos.x+ pos.width*2/3; j++) {
            if (fabs(xyz.at<Vec3f>(i, j)[2] - max_z) < FLT_EPSILON || fabs(xyz.at<Vec3f>(i, j)[2]) > max_z){continue;}

            else {
                //cout<<"x:"<<xyz.at<Vec3f>(i, j)[0]<<" y:"<<xyz.at<Vec3f>(i, j)[1]<<" z:"<<xyz.at<Vec3f>(i, j)[2]<<endl;;
                x = x + xyz.at<Vec3f>(i, j)[0];
                y = y + xyz.at<Vec3f>(i, j)[1];
                depth = depth + xyz.at<Vec3f>(i, j)[2];

                /*
                Point3f temp;
                temp.x=xyz.at<Vec3f>(i, j)[0];
                temp.y=xyz.at<Vec3f>(i, j)[1];
                temp.z=xyz.at<Vec3f>(i, j)[2];
                tempV.push_back(temp);
                */
            }
        }
    }
    /*
    Point3f clusterPos=sgbmMatching::cluster(tempV);
    tempV.clear();
    return clusterPos;
    */
    int numbers = pos.height*pos.width/9;
    //cout << "num:" << numbers<<endl;
    x = x / float(numbers);
    y = y / float(numbers);
    depth = depth / float(numbers);
    returnXYZ.x=x;
    returnXYZ.y=y;
    returnXYZ.z=depth;
    return returnXYZ;

}

