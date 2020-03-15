#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QString>
#include<QFileDialog>
#include<detector.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Image"),".",tr("Image Files (*.avi)"));
    qDebug()<<"filenames:"<<fileName;
    vioCapture.open(fileName.toLocal8Bit().data());
    if (vioCapture.isOpened())
        {
            int rate= vioCapture.get(CV_CAP_PROP_FPS);
            vioCapture >> frame;
            if (!frame.empty())
            {
                processImageDisp(frame);
                timer = new QTimer(this);
                timer->setInterval(1000/rate);   //set timer match with FPS
                connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
                timer->start();
            }
        }
}
void MainWindow::displayDemo(Mat image){
  QImage img=Mat2QImage(image);
  ui->label2->setPixmap(QPixmap::fromImage(img));
  ui->label2->resize(ui->label2->pixmap()->size());
}
void MainWindow::displayDet(Mat image){
  QImage img=Mat2QImage(image);
  ui->label->setPixmap(QPixmap::fromImage(img));
  ui->label->resize(ui->label->pixmap()->size());
}


void MainWindow::nextFrame(){
    vioCapture >> frame;
    if (!frame.empty())
    {
        processImageDisp(frame);
    }

}

QImage MainWindow::Mat2QImage(cv::Mat mat)
{
    if(mat.type() == CV_8UC1)
        {
            QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
            // Set the color table (used to translate colour indexes to qRgb values)
            image.setColorCount(256);
            for(int i = 0; i < 256; i++)
            {
                image.setColor(i, qRgb(i, i, i));
            }
            // Copy input Mat
            uchar *pSrc = mat.data;
            for(int row = 0; row < mat.rows; row ++)
            {
                uchar *pDest = image.scanLine(row);
                memcpy(pDest, pSrc, mat.cols);
                pSrc += mat.step;
            }
            return image;
        }
        // 8-bits unsigned, NO. OF CHANNELS = 3
        else if(mat.type() == CV_8UC3)
        {
            // Copy input Mat
            const uchar *pSrc = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return image.rgbSwapped();
        }
        else if(mat.type() == CV_8UC4)
        {
            qDebug() << "CV_8UC4";
            // Copy input Mat
            const uchar *pSrc = (const uchar*)mat.data;
            // Create QImage with same dimensions as input Mat
            QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
            return image.copy();
        }
        else
        {
            qDebug() << "ERROR: Mat could not be converted to QImage.";
            return QImage();
        }
}

void MainWindow::processImageSgbm(Mat & frame){
    vector<Mat> rectifiedImg= camera->RectifyImg(frame);
    std::vector<vector<float> > detections = detector->Detect(rectifiedImg[0]);
   // Mat disp =Matching->getDisp(rectifiedImg[0],rectifiedImg[1]);
    vector<Point3f> D3points;
    for(int i=0;i<detections.size();i++){
        vector<float>& d =detections[i];
        float score=d[2];
        int label=d[1];
        if(score>=confidence_threshold && label==1){
            int xmin=static_cast<int>(d[3] * rectifiedImg[0].cols);
            int ymin=static_cast<int>(d[4] * rectifiedImg[0].rows);
            int width=static_cast<int>(d[5] * rectifiedImg[0].cols)-static_cast<int>(d[3] * rectifiedImg[0].cols);
            int height=static_cast<int>(d[6] * rectifiedImg[0].rows)-static_cast<int>(d[4] * rectifiedImg[0].rows);
            cv::rectangle(rectifiedImg[0],Point(xmin,ymin),Point(xmin+width,ymin+height),Scalar(0,0,255),2);
            Rect pos=Rect(xmin,ymin,width,height);
            //qDebug()<<xmin<<" "<<ymin<<" "<<width<<" "<<endl;
            Point3f D3pos=Matching->get3Dpos(rectifiedImg[0],rectifiedImg[1],pos);
            D3points.push_back(D3pos);
        }

    }
    displayDet(rectifiedImg[0]);
    SimpleDemo *demo=new SimpleDemo(D3points);
    demo->showDemo();
    demo_img=demo->getDemo();
    //imshow("demo",demo_img);
    //waitKey(1);
    displayDemo(demo_img);
}


void MainWindow::processImageDisp(Mat & frame){
    vector<Mat> rectifiedImg= camera->RectifyImg(frame);
    std::vector<vector<float> > detections = detector->Detect(rectifiedImg[0]);
    Mat disp =matcher->Match(rectifiedImg[0],rectifiedImg[1]);
    vector<Point3f> D3points;
    for(int i=0;i<detections.size();i++){
        vector<float>& d =detections[i];
        float score=d[2];
        int label=d[1];
        if(score>=confidence_threshold && label==1){
            int xmin=static_cast<int>(d[3] * rectifiedImg[0].cols);
            int ymin=static_cast<int>(d[4] * rectifiedImg[0].rows);
            int width=static_cast<int>(d[5] * rectifiedImg[0].cols)-static_cast<int>(d[3] * rectifiedImg[0].cols);
            int height=static_cast<int>(d[6] * rectifiedImg[0].rows)-static_cast<int>(d[4] * rectifiedImg[0].rows);
            cv::rectangle(rectifiedImg[0],Point(xmin,ymin),Point(xmin+width,ymin+height),Scalar(0,0,255),2);
            Rect pos=Rect(xmin,ymin,width,height);
            Helper *func=new Helper(disp,pos,Q);
            Point3f D3pos=func->getD3pos();
            D3points.push_back(func->postProcess(D3pos));
        }

    }
    displayDet(rectifiedImg[0]);
    SimpleDemo *demo=new SimpleDemo(D3points);
    demo->showDemo();
    demo_img=demo->getDemo();
    //imshow("demo",demo_img);
    //waitKey(1);
    displayDemo(demo_img);
}

