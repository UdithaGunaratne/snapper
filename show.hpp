#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

int showImg(std:: string path, int time, int width, int height){

    std::string image_path = path;
    Mat resized_img;
    Size size(width, height);


    Mat img = imread(image_path, IMREAD_COLOR);

    if(img.empty()){
        std::cout << "Could not read the image : " << image_path << std::endl;
        return -1; 
    }

    resize(img, resized_img, size);


    namedWindow("snap", WND_PROP_FULLSCREEN);
    setWindowProperty("snap", WND_PROP_FULLSCREEN , WINDOW_FULLSCREEN);
    imshow("snap", resized_img);
    
    waitKey(time);
    destroyWindow("snap");
    
    return 0;
}
