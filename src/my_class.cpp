#include <Rcpp.h>
using namespace Rcpp;

class MyClass {
private:
    double x;

public:
    MyClass(double initialValue) : x(initialValue) {}

    void setX(double newX) {
        x = newX;
    }

    double getX() const {
        return x;
    }

    double square() const {
        return x * x;
    }
};

RCPP_MODULE(MyClassEx) {
    class_<MyClass>("MyClass")
    .constructor<double>()
    .method("setX", &MyClass::setX)
    .method("getX", &MyClass::getX)
    .method("square", &MyClass::square);
}
