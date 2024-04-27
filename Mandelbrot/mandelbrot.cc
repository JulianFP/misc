#include <cmath>
#include <iostream>
#include <deque>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>

class complex{
public:
    double re;
    double im;
    complex() : re(0), im(0){}
    complex(double real, double imaginary) : re(real), im(imaginary){}

    complex operator+(const complex &other){
        complex returnVal(re, im);
        returnVal.im += other.im;
        returnVal.re += other.re;
        return returnVal;
    }

    complex operator*(const complex &other){
        complex returnVal;
        returnVal.re = (re * other.re) - (im * other.im);
        returnVal.im = (re * other.im) + (im * other.re);
        return returnVal;
    }

    bool operator==(const complex &other){
        return (re == other.re) && (im == other.im);
    }

    double amount(){
        return std::sqrt((re*re) + (im*im));
    }
};

class canvas{
    std::deque<std::deque<bool>> _canvas;
    int _rows;
    int _cols;
    double _xDiff;
    double _yDiff;
    complex _origin;
    int _cursorRow;
public:
    canvas(complex origin, double width) : _origin(origin), _cursorRow(1){
        std::cout << '\n'; //make sure cursor is on new empty line

        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        _cols = size.ws_col;
        _rows = size.ws_row - 1;
        _xDiff = width / _cols;
        _yDiff = 2 * _xDiff;

        _canvas.resize(_rows);
        for(auto &row : _canvas){
            row.resize(_cols);
        }
    }

    complex getCoord(int col, int row) const{
        complex returnVal(_origin);

        returnVal.re -= _xDiff * (_cols/2);
        if(_cols % 2 == 0) returnVal.re += _xDiff / 2;
        //now at coordinates of most left pixel
        returnVal.re += _xDiff * col;

        returnVal.im -= _yDiff * (_rows/2);
        if(_rows % 2 == 0) returnVal.im += _yDiff / 2;
        //now at coordinates of lowest pixel
        returnVal.im += _yDiff * row;

        return returnVal;
    }

    bool getIn(int col, int row) const{
        return _canvas[row][col];
    }

    void setIn(int col, int row, bool value){
        _canvas[row][col] = value;
    }

    int getRow() const{
        return _rows;
    }

    int getCols() const{
        return _cols;
    }

    template<class Func>
    void apply(Func func){
        for(int row = 0; row < _rows; ++row){
            for(int col = 0; col < _cols; ++col){
                func(row, col);
            }
        }
    }

    template<class Func>
    void shiftDown(Func func){
        _canvas.pop_front();
        _canvas.push_back(std::deque<bool>(_cols));
        _origin.im += _yDiff; 
        for(int col = 0; col < _cols; ++col){
            func(_rows-1, col);
        }
    }

    template<class Func>
    void shiftUp(Func func){
        _canvas.pop_back();
        _canvas.push_front(std::deque<bool>(_cols));
        _origin.im -= _yDiff;
        for(int col = 0; col < _cols; ++col){
            func(0, col);
        }
    }

    template<class Func>
    void shiftRight(Func func){
        _origin.re += _xDiff;
        for(int row = 0; row < _rows; ++row){
            _canvas[row].pop_front();
            _canvas[row].push_back(false);
            func(row, _cols-1);
        }
    }

    template<class Func>
    void shiftLeft(Func func){
        _origin.re -= _xDiff;
        for(int row = 0; row < _rows; ++row){
            _canvas[row].pop_back();
            _canvas[row].push_front(false);
            func(row, 0);
        }
    }

    template<class Func>
    void scaleWidth(double factor, Func func){
        _xDiff = factor * _xDiff;
        _yDiff = 2 * _xDiff;
        apply(func);
    }

    void drawCanvas(){
        while(_cursorRow > 1){
            std::cout << "\e[A";
            --_cursorRow;
        }
        for(int row = 0; row < _rows; ++row){
            for(int col = 0; col < _cols; ++col){
                if(_canvas[row][col]) std::cout << "\u2588";
                else std::cout << ' ';
            }
            std::cout << '\n';
            ++_cursorRow;
        }
    }
};

bool mandelbrot(const complex &c, int iterations){
    complex z;
    for(int n = 1; n <= iterations; ++n){
        z = (z*z)+c;
        if(z.amount() > 2) return false;
    }
    return true;
}

int main(){
    complex origin;
    double width;
    int iterations;
    char nextchar;
    std::cout << "Enter origin point (x and y): " << std::flush;
    std::cin >> origin.re;
    std::cin >> origin.im;
    std::cout << "Enter width of Gaussian plane that should be displayed: " << std::flush;
    std::cin >> width;
    std::cout << "Enter amount of iterations per pixel: " << std::flush;
    std::cin >> iterations;

    canvas terminal(origin, width);
    auto mandelbrotLambda = [&](int row, int col){terminal.setIn(col, row, mandelbrot(terminal.getCoord(col, row), iterations));};
    terminal.apply(mandelbrotLambda);
    terminal.drawCanvas();

    // Save the current terminal attributes
    struct termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    // Set the terminal to raw mode
    struct termios newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while(nextchar != 'q'){
        if (read(STDIN_FILENO, &nextchar, 1) < 0) {
            perror("read");
            break;
        }
        switch(nextchar){
            case 'j':
                terminal.shiftDown(mandelbrotLambda);
                break;
            case 'k':
                terminal.shiftUp(mandelbrotLambda);
                break;
            case 'l':
                terminal.shiftRight(mandelbrotLambda);
                break;
            case 'h':
                terminal.shiftLeft(mandelbrotLambda);
                break;
            case 'i':
                terminal.scaleWidth(0.5, mandelbrotLambda);
                break;
            case 'o':
                terminal.scaleWidth(2, mandelbrotLambda);
                break;
            default:
                continue;
        }
        terminal.drawCanvas();
    }
    // Restore the terminal attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}
