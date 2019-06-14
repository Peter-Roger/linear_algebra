#include <iostream>
#include <random>
#include "linear_algebra.hpp"

using namespace linear_algebra;

void random_float_fill(Matrix<float> &matrix) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> real_dist(-9, 9);

    for (int i = 0; i < matrix.rows(); i++) {
        for (int j = 0; j < matrix.cols(); j++) {
            matrix.set(i, j, real_dist(gen));
        }
    }
}

int main()
{
    Matrix<float> m1(5, 7);
    Matrix<float> m2(7, 5);
    Matrix<float> m3(5, 5);

    random_float_fill(m1);
    random_float_fill(m2);
    random_float_fill(m3);

    std::cout << trans(m1 * m2) * m3 << std::endl;
    std::cout << trans(trans(mult(m1, m2) * m3)) << std::endl;
    return 0;
}