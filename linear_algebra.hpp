#include <algorithm>
#include <iostream>

namespace linear_algebra {

    /*
    * Matrix Expression Template Base Class
    */
    template<typename T, typename E>
    class MatrixExpression {
        typedef T value_type;
        typedef E expr_type;

    public:

        size_t rows() const {
            return rows_;
        }

        size_t cols() const {
            return cols_;
        }

        size_t size() const {
            return rows_ * cols_;
        }

        void set_dimension(size_t rows, size_t cols) {
            rows_ = rows;
            cols_ = cols;
        }

        /*
        * Calls () operator on derived class.
        */
        value_type operator () (size_t row, size_t col) const {
            return static_cast<expr_type const&>(*this)(row, col);
        }

        friend std::ostream& operator << (std::ostream& stream, const MatrixExpression<value_type, expr_type> & expr)  {
            if (expr.size() > 0) {
                for (size_t i = 0; i < expr.rows(); ++i) {
                    for (size_t j = 0; j < expr.cols(); ++j) {
                        stream << expr(i, j) << ' ';
                    }
                    stream << '\n';
                }
            }
            return stream;
        }

    private:
        size_t rows_;
        size_t cols_;
    };

    /*
    * An NxM Matrix of values of type T.
    *
    * T the type of object stored in the Matrix (int, double, float, etc...)
    */
    template<typename T>
    class Matrix : public MatrixExpression<T, Matrix<T>> {
        typedef T value_type;

    public:

        /*
        * Default constructor.
        */
        Matrix() {
            init(0,0);
        }

        /*
        * Standard constructor.
        */
        Matrix(size_t rows, size_t cols) {
            init(rows, cols);
        }

        /*
        * Copy constructor.
        */
        Matrix(Matrix<value_type> &orig) {
            copy(orig);
        }

        /*
        * Constucts matrix from and matrix expression, forcing its evaluation.
        */
        template<typename E>
        Matrix(MatrixExpression<value_type, E> const& expr) {
            copy(expr);
        }

        /*
        * Initializer list constructor.
        */
        template<typename T2>
        Matrix(const std::initializer_list<std::initializer_list<T2>> list) {
            resize_(list.size(), list.begin()->size());
            size_t i = 0, j = 0;
            for (auto row : list) {
                j = 0;
                for (T2 element : row) {
                    data_[i * this->cols() + j] = static_cast<value_type>(element);
                    ++j;
                }
                ++i;
            }
        }

        /*
        * Destructor.
        */
        ~Matrix() {
            if (data_ != nullptr) {
                delete[] data_;
            }
        }

        /*
        * Copy assignment operator.
        */
        Matrix<value_type>& operator= (Matrix<value_type> const& orig) {
            if (&orig != this) {
                copy(orig);
            }
            return *this;
        }

        /*
        * Assign value as the element in the matrix at the location
        * specified by row and col, with bounds checking.
        */
        void set(size_t row, size_t col, value_type value) {
            check_bounds(row, col);
            data_[row * this->cols() + col] = value;
        }

        /*
        * Returns the element in the matrix at the location
        * specified by row and col, with bounds checking.
        *
        * If row or col is not within the range of the matrix, an exception
        * of type std::out_of_range is thrown.
        */
        value_type operator () (size_t row, size_t col) const {
            check_bounds(row, col);
            return data_[row * this->cols() + col];
        }

        /*
        * Returns a reference to the element in the matrix at the location
        * specified by row and col, with bounds checking.
        *
        * If row or col is not within the range of the matrix, an exception
        * of type std::out_of_range is thrown.
        */
        value_type& operator () (size_t row, size_t col) {
            check_bounds(row, col);
            return data_[row * this->cols() + col];
        }

    protected:

        /*
        * Initialize matrix with default values.
        */
        void init(size_t rows, size_t cols) {
            resize_(rows, cols);
            std::fill_n(data_, this->size(), value_type());
        }

        /*
        * Copies in matrix/matrix expression.
        */
        template<typename T2, typename E>
        void copy(MatrixExpression<T2, E> const& expr) {
            resize_(expr.rows(), expr.cols());
            // std::cout << expr.rows() << " " << expr.cols() << std::endl;
            for (size_t row = 0; row < this->rows(); ++row) {
                for (size_t col = 0; col < this->cols(); ++col) {
                    data_[row * this->cols() + col] = static_cast<value_type>(expr(row, col));
                } 
            }
        }

        /*
        * Resizes matrix to new specified dimensions.
        */
        void resize_(size_t m, size_t n) {
            if (data_ != nullptr && (this->rows() != m || this->cols() != n)) {
                delete[] data_;
                data_ = nullptr;
            }

            if (data_ == nullptr) {
                this->set_dimension(m, n);
                data_ = new value_type[this->rows() * this->cols()];
            }
        }

        /*
        * Checks that indexes i and j are within the matrix bounds.
        *
        * Throws out_of_range exception for invalid indices.
        */
        void check_bounds(size_t i, size_t j) const {
            if ((i >= this->rows() || j >= this->cols()) || (i < 0 || j < 0 )) {
                throw std::out_of_range("Index out of bounds.");
            }
        }

    private:
        value_type* data_ = nullptr;
    };


    /*
    * Addition Expression Template.
    *
    * T the value type
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    class Addition : public MatrixExpression<T, Addition<T, E1, E2>> {
        typedef T value_type;
        typedef E1 left_expr;
        typedef E2 right_expr;

        public:
            Addition(left_expr const& left, right_expr const& right)
            : left_operand(left), right_operand(right) {
                check_dimensions(left_operand, right_operand);
                this->set_dimension(left_operand.rows(), right_operand.cols());
            }

            value_type operator () (size_t row, size_t col) const {
                return left_operand(row, col) + right_operand(row, col);
            }

        private:
            left_expr const& left_operand;
            right_expr const& right_operand;

        /*
        * Checks that matrix addition is defined for the left and
        * right matrices.
        *
        * Throws a std::logic_error if matrix addition is undefined.
        */
        void check_dimensions(left_expr const& left_operand, right_expr const& right_operand) {
            if (left_operand.rows() != right_operand.rows() ||
                left_operand.cols() != right_operand.cols()) {
                throw std::logic_error("Matrix addition is only defined " \
                    "when the dimensions of the left-hand matrix is equal " \
                    "to the dimensions of the right-hand matrix."); 
            }
        }
    };

    /*
    * Matrix addition.
    *
    * T the value type
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    Addition<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>
    add(MatrixExpression<T, E1> const& left, MatrixExpression<T, E2> const& right) {
        return Addition<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>(left, right);
    }

    /*
    * Overload + operator for Multiplication Expression Template.
    *
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    Addition<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>
    operator+(MatrixExpression<T, E1> const& left, MatrixExpression<T, E2> const& right) {
        return Addition<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>(left, right);
    }


    /*
    * Multiplication Expression Template.
    *
    * T the value type
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    class Multiplication : public MatrixExpression<T, Multiplication<T, E1, E2>> {
        typedef T value_type;
        typedef E1 left_expr;
        typedef E2 right_expr;

    public:

        Multiplication(left_expr const& left, right_expr const& right)
        : left_operand(left), right_operand(right) {
            check_dimensions(left_operand, right_operand);
            this->set_dimension(left_operand.rows(), right_operand.cols());
        }

        /**
        * Computes the dot-product of the row vector in the left operand
        * specified by 'row' and the column vector in the right operand
        * specified by 'col'.
        *
        * row the index of the row in the left operand
        * col the index of the column in the right operand 
        */
        value_type operator () (size_t row, size_t col) const {
            value_type dot_product = value_type();
            for (size_t i = 0; i < left_operand.cols(); ++i) {
                dot_product += left_operand(row, i) * right_operand(i, col);
            }
            return dot_product;
        }

    private:
        left_expr const& left_operand;
        right_expr const& right_operand;

        /*
        * Checks that matrix multiplication is defined for the left and
        * right matrices.
        *
        * Throws a std::logic_error if matrix multiplication is undefined.
        */
        void check_dimensions(left_expr const& left_operand, right_expr const& right_operand) {
            if (left_operand.cols() != right_operand.rows()) {
                throw std::logic_error("Matrix multiplication is only defined " \
                    "when the number of columns in the left-hand matrix is equal " \
                    "to the number of rows in the right-hand matrix."); 
            }
        }

    };

    /*
    * Matrix multiplication.
    *
    * T the value type
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    Multiplication<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>
    mult(MatrixExpression<T, E1> const& left, MatrixExpression<T, E2> const& right) {
        return Multiplication<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>(left, right);
    }

    /*
    * Overload * operator for Multiplication Expression Template.
    *
    * E1 the type of Expression of the left-hand operand.
    * E2 the type of Expression of the right-hand operand.
    */
    template<typename T, typename E1, typename E2>
    Multiplication<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>
    operator*(MatrixExpression<T, E1> const& left, MatrixExpression<T, E2> const& right) {
        return Multiplication<T, MatrixExpression<T, E1>, MatrixExpression<T, E2>>(left, right);
    }

    /*
    * Transpose Expression Template.
    *
    * T the value type.
    * E the expression type.
    */
    template<typename T, typename E>
    class Transpose : public MatrixExpression<T, Transpose<T, E>> {
        typedef T value_type;
        typedef E expr_type;

    public:

        Transpose(expr_type const& operand) : operand_(operand) {
            // row and column dimension is swapped
            this->set_dimension(operand.cols(), operand.rows());
        }

        value_type operator () (size_t row, size_t col) const {
            return operand_(col, row);
        }

    private:
        expr_type const& operand_;
    };

    /*
    * Transpose Matrix.
    *
    * E the type of Expression of the operand.
    */
    template<typename T, typename E>
    Transpose<T, MatrixExpression<T, E>> trans(MatrixExpression<T, E> const& operand) {
        return Transpose<T, MatrixExpression<T, E>>(operand);
    }
}