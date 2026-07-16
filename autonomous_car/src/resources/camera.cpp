#include "camera.hpp"


Camera::Camera(unsigned int ind, unsigned int apiID){
         
        std::string index = "/dev/video" + std::to_string(ind);
        std::cout << "Opening " + index + "..." << std::endl;
        cap.open(ind, apiID);
        if(!cap.isOpened())
        {
                std::cerr << "Can't access camera" << std::endl;
        }
}

void Camera::setup(unsigned int width, unsigned int height){

        if(!cap.isOpened()){
                std::cerr << "Camera not opened, setup not applied" << std::endl;
                return;
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

        //Set up directly camera parameters
        //std::system("v4l2-ctl --set-ctrl white_balance_automatic=1");
        //std::system("v4l2-ctl --set-ctrl gain_automatic=1");
        //std::system("v4l2-ctl --set-ctrl auto_exposure=0");

        std::cout << "Setup Done!" << std::endl;
}


cv::Mat Camera::getFrame()
{
    cv::Mat frame;
    if(cap.read(frame))
    {       
        return frame;
    }
    return cv::Mat();
}













