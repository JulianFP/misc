#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <chrono>
#include <thread>
#include <csignal>
#include <sys/ioctl.h>
#include <unistd.h>


void moveCursorDown(int rows){
    std::cout << "\033[" << rows << "B" << "\r";
}


void moveCursorRight(int cols){
    std::cout << "\033[" << cols << "C";
}


void drawLine(const cv::Mat &binaryFrame, const int &row, const int &margin){
    if(margin > 0){
        //draw left margin
        moveCursorRight(margin);
        //remove everything before cursor
        std::cout << "\033[1K";
    }

    //draw frame
    for(int col = 0; col < binaryFrame.cols*binaryFrame.channels(); col+=binaryFrame.channels()){
        if(binaryFrame.at<uchar>(row, col) != 0 && binaryFrame.at<uchar>(row+1, col) != 0) std::cout << "\u2588";
        else if(binaryFrame.at<uchar>(row, col) == 0 && binaryFrame.at<uchar>(row+1, col) != 0) std::cout << "\u2584";
        else if(binaryFrame.at<uchar>(row, col) != 0 && binaryFrame.at<uchar>(row+1, col) == 0) std::cout << "\u2580";
        else std::cout << ' ';
    }

    //remove everything after cursor
    std::cout << "\033[0K";
}


int main (int argc, char *argv[]) {
    if(argc < 2){
        std::cout << "You need to provide a path to the video file to play as the first cli argument";
        return 1;
    }
    cv::VideoCapture cap(argv[1]);

    if(!cap.isOpened()){
        std::cout << "The provided path/file was invalid";
        return 1;
    }

    std::chrono::milliseconds frametime(static_cast<int>(1000.0 / cap.get(cv::CAP_PROP_FPS)));

    //clear terminal
    std::cout << "\033[2J";
    //hide cursor and show it again at exit
    std::cout << "\033[?25l";
    std::signal(SIGINT, [](int signal) { std::cout << "\033[?25h"; std::exit(0); });

    //start audio play
    sf::Music music;
    if(argc > 2){
        music.openFromFile(std::string(argv[2]));
        music.play();
    }

    while(true){
        const auto frameEnd = std::chrono::steady_clock::now() + frametime;

        cv::Mat frame;
        cap >> frame;
        if(frame.empty()) break;

        //get terminal resolution
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        int maxWidth = size.ws_col;
        //multiply by two since we want to map two pixels to one character in terminal
        int maxHeight = 2*(size.ws_row); 

        //resize image to terminal resolution
        cv::Mat frameResized;
        float scaleWidth = (float) maxWidth / (float) frame.cols;
        float scaleHeight = (float) maxHeight / (float) frame.rows;
        float scale = std::min(scaleWidth, scaleHeight);
        cv::resize(frame, frameResized, cv::Size(), scale, scale, cv::INTER_AREA);

        //apply thresholding to convert to binary image
        cv::Mat binaryFrame;
        cv::threshold(frameResized, binaryFrame, 128, 255, cv::THRESH_BINARY);

        int horMargin = (maxWidth - binaryFrame.cols) / 2;
        int vertMargin = (maxHeight - binaryFrame.rows) / 4; //divide by 4 because two pixels per char
        
        //move cursor to home position
        std::cout << "\033[H";

        if(vertMargin > 0){
            //draw top margin
            moveCursorDown(vertMargin);
            //remove everything before the cursor
            std::cout << "\033[1J";
        }

        for(int row = 0; row < binaryFrame.rows-2; row+=2){
            drawLine(binaryFrame, row, horMargin);

            moveCursorDown(1);
        }
        drawLine(binaryFrame, binaryFrame.rows-2, horMargin);

        //remove everything after the cursor
        if(vertMargin > 0) std::cout << "\033[0J";

        //flush frame 
        std::cout << std::flush;

        if(std::chrono::steady_clock::now() >= frameEnd){
            std::cout << "It took to long to render a frame! Decrease resolution by zooming in";
            return 1;
        }
        std::this_thread::sleep_until(frameEnd);
    }

    cap.release();
    std::cout << "\033[?25l"; //show cursor again
    return 0;
}
