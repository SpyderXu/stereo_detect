#include "matcher.h"

Matcher::Matcher()
{

}
cv::Mat Matcher::getColorDisp(){
    return color_disp;
}
Matcher::Matcher(const string & model_file,
            const string & weights_file,
            const string & mean_file,
            const string & mean_value){

    Caffe::set_mode(Caffe::GPU);

    net_.reset(new Net<float>(model_file,TEST));
    net_->CopyTrainedLayersFrom(weights_file);

    CHECK_EQ(net_->num_inputs(),2)<<"Network should have two input";

    Blob<float>* input_layer0=net_->input_blobs()[0];
    num_channels_=input_layer0->channels();

    input_geometry_ = cv::Size(input_layer0->width(), input_layer0->height());
}
void Matcher::WrapInputLayer(std::vector<cv::Mat>* input_channels0,std::vector<cv::Mat>* input_channels1){
    Blob<float> * input_layer0=net_->input_blobs()[0];

    int width=input_layer0->width();
    int height=input_layer0->height();

    float *input_data0=input_layer0->mutable_cpu_data();
    for (int i=0;i<input_layer0->channels();i++){
        cv::Mat channel0(height,width,CV_32FC1,input_data0);
        input_channels0->push_back(channel0);
        input_data0 +=width*height;
    }

    Blob<float> * input_layer1=net_->input_blobs()[1];


    float *input_data1=input_layer1->mutable_cpu_data();
    for(int i=0;i<input_layer1->channels();i++){
        cv::Mat channel1(height,width,CV_32FC1,input_data1);
        input_channels1->push_back(channel1);
        input_data1+=width*height;
    }
}

void Matcher::Preprocess(cv::Mat &img0,cv::Mat img1, std::vector<cv::Mat> *input_channels0,std::vector<cv::Mat> * input_channels1){
    cv::Mat sample_float0,sample_float1;

    if (num_channels_ == 3){
        img0.convertTo(sample_float0, CV_32FC3);
        img1.convertTo(sample_float1, CV_32FC3);
    }
    else{
        img0.convertTo(sample_float0, CV_32FC3);
        img1.convertTo(sample_float1, CV_32FC3);
    }
    cv::split(sample_float0, *input_channels0);
    cv::split(sample_float1, *input_channels1);

    CHECK(reinterpret_cast<float*>(input_channels0->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}


cv::Mat Matcher::Match(cv::Mat &Limg,cv::Mat &Rimg){
    Blob<float>* input_layer0= net_->input_blobs()[0];
    input_layer0->Reshape(1, num_channels_,input_geometry_.height, input_geometry_.width);

    Blob<float>* input_layer1= net_->input_blobs()[0];
    input_layer1->Reshape(1, num_channels_,input_geometry_.height, input_geometry_.width);

    std::vector<cv::Mat> input_channels0;
    std::vector<cv::Mat> input_channels1;

    WrapInputLayer(&input_channels0,&input_channels1);
    Preprocess(Limg,Rimg, &input_channels0,&input_channels1);
    net_->Forward();

    Blob<float> * result_blob=net_->output_blobs()[0];

    int width=result_blob->width();
    int height=result_blob->height();
    int channels=result_blob->channels();
    cv::Mat Disp;
    Disp=Mat(height,width,CV_32FC1);
    color_disp=Mat(height,width,CV_8UC1);
    const float* result=result_blob->cpu_data();
    //cv::Mat Disp(height,width,CV_32FC1,result);

    long k=0;
    for(int i=0;i<height;i++){
        for (int j=0;j<width;j++){
            Disp.at<float>(i,j)=abs(result[k]);
            color_disp.at<uchar>(i,j)=abs(result[k])*4;
            //cout<<"result:"<<result[k]<<endl;
            k++;
        }
    }
    return Disp;
}

