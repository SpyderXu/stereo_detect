#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include<QImage>
#include"camera.h"
#include"sgbmmatching.h"
#include"detector.h"
#include"simpledemo.h"
#include"matcher.h"
#include"helper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void displayDet(Mat image);
    void displayDemo(Mat image);
    void nextFrame();
    QImage Mat2QImage(cv::Mat cvImg);
    void processImageSgbm(Mat & frame);
    void processImageDisp(Mat & frame);

private:
    const string& disp_model_file = "/home/cvpr/xcz/dispflownet-release/models/DispNetCorr1D-K/deploy.tpl.prototxt";
    const string& disp_weights_file = "/home/cvpr/xcz/dispflownet-release/models/DispNetCorr1D-K/model/DispNetCorr1D-K_CVPR2016.caffemodel";
    const string& disp_mean_file = "";
    const string& disp_mean_value ="104,117,123";
    const string& disp_out_file = "output.txt";
    Ui::MainWindow *ui;
    Camera *camera=new Camera("/home/cvpr/ShanFu/data/1212/intrinsics.yml","/home/cvpr/ShanFu/data/1212/extrinsics.yml");
    Mat Q=camera->getMatQ();
    sgbmMatching *Matching=new sgbmMatching(Q,8,0,2,2);
    const string model_file = "/home/cvpr/caffe/models/KITTI/SSD_300x300/deploy.prototxt";
    const string weights_file = "/home/cvpr/caffe/models/KITTI/SSD_300x300/VGG_KITTI_SSD_300x300_iter_80000.caffemodel";
    const string mean_file="";
    const string mean_value = "104,117,123";
    const string& out_file = "/home/cvpr/caffe/examples/ssd/output.txt";
    const string& out_dir="/home/cvpr/caffe/examples/ssd/output/output.txt";
    const float confidence_threshold = 0.3;
    Detector *detector=new Detector(model_file, weights_file, mean_file, mean_value);
    Matcher *matcher=new Matcher(disp_model_file,disp_weights_file,disp_mean_file,disp_mean_value);
    QTimer *timer;
    VideoCapture vioCapture;
    Mat imgL;
    Mat imgR;
    Mat frame;
    Mat detect_img;
    Mat demo_img;
};

#endif // MAINWINDOW_H
