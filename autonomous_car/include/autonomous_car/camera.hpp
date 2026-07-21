#include <iostream>
#include <cstdbool>
#include <cstdlib>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>

class Camera
{
public:
        Camera(unsigned int index = 0, unsigned int apiID = cv::CAP_ANY);
        void setup( unsigned int width, unsigned int height);
        cv::Mat getFrame();
        bool isOpened() const { return cap.isOpened(); };

protected:
        cv::VideoCapture cap;
};
