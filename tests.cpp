#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <stdexcept>
#include "linear_algebra.hpp"

using namespace linear_algebra;

class TestMatrix {

public:

    void run() {
        std::cout << "Running tests:" << std::endl;
        test_dft_ctor();
        test_std_ctor();
        test_cpy_ctor();
        test_list_init_ctor_sq();
        test_list_init_ctor_rec();
        test_cpy_asnmt_op();
        test_call_out_of_bounds();
        test_set();
        test_transpose_NxN();
        test_mult_NxN();
        test_transpose_NxM();
        test_nested_transpose_NxM();
        test_mult_NxN();
        test_mult_NxM();
        test_mult_3_terms();
        test_trans_mult_3_terms();
        test_trans_matrix_mult();
        test_dft_ctor();
        if (all_cases_passed) {
            std::cout << std::endl << "All test cases passed." << std::endl;
        } else {
            std::cout << std::endl << oss.str();
        }
    }

    //-------------------- TEST MATRIX CLASS --------------------

    void test_dft_ctor() {
        std::string prompt = __func__;
        Matrix<int> m1;
        bool condition;
        try {
            m1.set(0,0,5);
            condition = false;
        } catch (const std::out_of_range&) {
            condition = m1.rows() == 0 && m1.cols() == 0 && m1.size() == 0;
        }
        test(condition, prompt);
    }

    void test_std_ctor() {
        std::string prompt = __func__;
        int n = 5, m = 5;
        Matrix<int> m1(n,m);
        bool condition = m1.rows() == n && m1.cols() == m && m1.size() == n*m;
        test(condition, prompt);
    }

    void test_cpy_ctor() {
        std::string prompt = __func__;
        Matrix<int> m1(6,7);
        random_int_fill(m1);
        Matrix<int> m2(m1);
        bool condition = matrix_equal(m1, m2);
        test(condition, prompt);
    }

    void test_list_init_ctor_sq() {
        std::string prompt = __func__;
        Matrix<double> m1({{1.1, 2.2}, {3.3, 4.4}});
        bool condition = m1(0,0) == 1.1 && m1(0,1) == 2.2 && m1(1,0) == 3.3 && m1(1,1) == 4.4;
        test(condition, prompt);
    }

    void test_list_init_ctor_rec() {
        std::string prompt = __func__;
        Matrix<double> m1({{1.1}, {2.2}});
        bool condition = m1(0,0) == 1.1 && m1(1,0) == 2.2;
        test(condition, prompt);
    }

    void test_cpy_asnmt_op() {
        std::string prompt = __func__;
        Matrix<int> m1(100, 100);
        random_int_fill(m1);
        Matrix<int> m2 = m1;
        bool condition = matrix_equal(m1, m2);
        test(condition, prompt);
    }

    void test_call_out_of_bounds() {
        std::string prompt = __func__;
        Matrix<int> m1(5,5);
        random_int_fill(m1);
        bool condition = false;

        try {
            m1(5,5);
        } catch (const std::out_of_range& e) {
            condition = true;
        }
        test(condition, prompt);
    }

    void test_set() {
        std::string prompt = __func__;
        Matrix<int> m1(5,5);
        m1.set(3,2,10);
        bool condition = m1(3,2) == 10;
        test(condition, prompt);
    }

    //-------------------- TEST MATRIX OPERATIONS --------------------

    void test_transpose_NxN() {
        std::string prompt = __func__;
        int n = 10;
        Matrix<int> m1(n,n);
        random_int_fill(m1);
        Matrix<int> t = trans(m1);
        bool condition = true;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                condition = t(i, j) == m1(j, i);
                if (!(condition)) {
                    break;
                }
            }
        }
        test(condition, prompt);
    }

    void test_transpose_NxM() {
        std::string prompt = __func__;
        int n = 10, m = 15;
        Matrix<int> m1(n, m);
        random_int_fill(m1);
        Matrix<int> t = trans(m1);
        bool condition = true;
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                condition = t(i, j) == m1(j, i);
                if (!(condition)) {
                    break;
                }
            }
        }
        test(condition, prompt);
    }

    void test_nested_transpose_NxM() {
        std::string prompt = __func__;
        int n = 2, m = 5;
        Matrix<int> m1(n, m);
        random_int_fill(m1);
        Matrix<int> t = trans(m1);
        Matrix<int> tt = trans(trans(m1));
        bool condition = matrix_equal(tt, m1);
        test(condition, prompt);
    }

    void test_mult_op_NxN() {
        std::string prompt = __func__;
        Matrix<int> expected({{348,372,334,197},
                              {495,462,407,201},
                              {376,435,357,245},
                              {132,122,116,55}});

        Matrix<int> m1({{10,6,5,9},
                        {3,2,21,15},
                        {8,7,8,4},
                        {2,1,4,5}});

        Matrix<int> m2({{7,8,3,0},
                        {24,33,31,27},
                        {16,17,11,7},
                        {6,1,7,0}});

        Matrix<int> res = m1 * m2;
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

    void test_mult_NxN() {
        std::string prompt = __func__;
        Matrix<int> expected({{348,372,334,197},
                              {495,462,407,201},
                              {376,435,357,245},
                              {132,122,116,55}});

        Matrix<int> m1({{10,6,5,9},
                        {3,2,21,15},
                        {8,7,8,4},
                        {2,1,4,5}});

        Matrix<int> m2({{7,8,3,0},
                        {24,33,31,27},
                        {16,17,11,7},
                        {6,1,7,0}});

        Matrix<int> res = mult(m1, m2);
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

    void test_mult_NxM() {
        std::string prompt = __func__;
        Matrix<int> expected({{348,372,334},
                              {495,462,407},
                              {376,435,357}});
        Matrix<int> m1({{10,6,5,9},
                        {3,2,21,15},
                        {8,7,8,4}});
        Matrix<int> m2({{7,8,3},
                        {24,33,31},
                        {16,17,11},
                        {6,1,7}});
        Matrix<int> res = m1 * m2;
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

    void test_mult_3_terms() {
        std::string prompt = __func__;
        Matrix<int> expected({{8018,10342,5646,3072},
                              {10186,12947,7491, 3894},
                              {8852,11332,6254, 3329}});
        Matrix<int> m1({{10,6,5,9},
                        {3,2,21,15},
                        {8,7,8,4}});
        Matrix<int> m2({{7,8,3},
                        {24,33,31},
                        {16,17,11},
                        {6,1,7}});
        Matrix<int> m3({{5,4,8,2},
                        {7,7,5,1},
                        {11,19,3,6}});
        Matrix<int> res = m1 * m2 * m3;
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

    void test_trans_mult_3_terms() {
        std::string prompt = __func__;
         Matrix<int> expected({{8018, 10186, 8852},
                               {10342, 12947, 11332},
                               {5646, 7491, 6254},
                               {3072, 3894, 3329}});


        Matrix<int> m1({{10,6,5,9},
                        {3,2,21,15},
                        {8,7,8,4}});
        Matrix<int> m2({{7,8,3},
                        {24,33,31},
                        {16,17,11},
                        {6,1,7}});
        Matrix<int> m3({{5,4,8,2},
                        {7,7,5,1},
                        {11,19,3,6}});
        Matrix<int> res = trans(m1 * m2 * m3);
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

    void test_trans_matrix_mult() {
        std::string prompt = __func__;
        Matrix<int> expected({{64, 50, 14},
                              {32, 25, 7},
                              {12, 11, 3},
                              {34, 16, 5}});
        Matrix<int> m1({{6,3,1,4},
                        {8,4,2,1}});
        Matrix<int> m2({{8,3,1},
                        {2,4,1}});
        Matrix<int> res = trans(m1) * m2;
        bool condition = matrix_equal(res, expected);
        test(condition, prompt);
    }

protected:

    void test(bool condition, const std::string& prompt) {
        if (condition) {
            std::cout << "✓";
        } else {
            all_cases_passed = false;
            std::cout << "F";
            oss << "TEST FAILED: " << prompt << std::endl;
        }
    }

    template<typename T>
    bool matrix_equal(Matrix<T> m1, Matrix<T> m2) {
        if (m1.rows() != m2.rows() || m1.cols() != m2.cols()) {
            return false;
        } else {
            for (int i = 0; i < m1.rows(); ++i) {
                for (int j = 0; j < m1.cols(); ++j) {
                    if (m1(i, j) != m2(i, j)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void random_int_fill(Matrix<int> &matrix) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> uni_dist(-99, 99);

        for (int i = 0; i < matrix.rows(); i++) {
            for (int j = 0; j < matrix.cols(); j++) {
                matrix.set(i, j, uni_dist(gen));
            }
        }
    }

private:
    std::ostringstream oss;
    bool all_cases_passed = true;
};

int main() {
    TestMatrix suite;
    suite.run();
};