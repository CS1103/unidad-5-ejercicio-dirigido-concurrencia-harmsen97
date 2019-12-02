#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <fstream>

int numRow=0;
int sizeA;

std::vector<double> vec1;


template <class T>
class Matrix{

private:
    T** m;
    int rows,cols;

public:
    Matrix(int _rows, int _cols){
        rows=_rows;
        cols=_cols;

        m = new T*[rows];
        for(int i = 0; i < rows; i++)
            m[i] = new T[cols];

        sizeA=cols*rows;
    }

    Matrix(std::string path) {

        numRow++;

        T numb;

        std::ifstream file1;
        file1.open(path);

        int cont = 0;

        if(numRow==1){
            while (file1 >> numb) {
                if (cont == 0)
                    rows = numb;
                if (cont == 1) {
                    cols = numb;
                    m = new T *[rows];
                    for (int i = 0; i < rows; i++)
                        m[i] = new T[cols];
                }
                if (cont > 1) {
                    vec1.push_back(numb);
                }

                cont++;
            }


            int k = 0;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    m[i][j] = vec1[k];
                    k++;
                }
            }

            sizeA=cols*rows;
        }

        if(numRow==2){

            rows=vec1[0+sizeA];
            cols=vec1[1+sizeA];

            std::cout<<"rows: "<<rows<<std::endl;
            std::cout<<"cols: "<<cols<<std::endl;
            m = new T *[rows];
            for (int i = 0; i < rows; i++)
                m[i] = new T[cols];

            int k = 2;
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    m[i][j]=vec1[k+sizeA];
                    k++;
                }
            }

        }

        file1.close();

    }

    T** getMatrix(){
        return m;
    }

    void printMatrix(){
        std::cout << "Matrix" << std::endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++)
                std::cout << m[i][j] << " ";
            std::cout << std::endl;
        }
    }
    Matrix operator * (const Matrix m2){
        Matrix product = Matrix(rows,m2.cols);
        std::vector<std::thread> vectorThread(rows);

        T total=0;
        auto multi = [&product,&total,&m2,this](int i){
            for (int j = 0; j < m2.cols; j++) {
                for (int k = 0; k < this->cols; k++) {
                    total = (this->getMatrix()[i][k]) * (m2.m[k][j]) + total;
                }
                product.getMatrix()[i][j] = total;
                total = 0;
            }
        };

        for (int l = 0; l < rows; l++) {
            vectorThread[l] = std::thread(multi,l);
        }

        for(int i = 0; i<rows; i++)
            vectorThread[i].join();


        return product;
    }


};

int main(){

    Matrix<float> m1 = Matrix<float>("test.txt");
    Matrix<float> m2 = Matrix<float>("test.txt");

    m1.printMatrix();
    m2.printMatrix();


    Matrix<float> product = m1*m2;
    product.printMatrix();



    return 0;
}