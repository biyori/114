#include <iostream>
#include <thread>
#include <vector>

using namespace std;

struct Matrix
{
    int cols = 0;
    int rows = 0;
    int **data;

    Matrix() {}

    Matrix(int r, int c)
    {
        cols = c;
        rows = r;
        data = new int *[rows];

        for (int i = 0; i < rows; i++)
        {
            data[i] = new int[cols];

            for (int j = 0; j < cols; j++)
            {
                data[i][j] = 0;
            }
        }
    }
};

void rowMultiply(Matrix m1, Matrix m2, Matrix mOutput, int row)
{
    for (int i = 0; i < m2.cols; i++)
    {
        for (int j = 0; j < m1.cols; j++)
        {
            mOutput.data[row][i] += (m1.data[row][j] * m2.data[j][i]);
        }
    }
}

Matrix dotProduct(Matrix m1, Matrix m2)
{
    Matrix outM;

    vector<thread> threads;          // declare a vector of threads
    outM = Matrix(m1.rows, m2.cols); // set output matrix rows and cols

    for (int i = 0; i < m1.rows; i++)
    {
        threads.emplace_back(rowMultiply, m1, m2, outM, i); // emplace_back vs push_back https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back
    }
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join(); // join threads
    }
    return outM;
}

void printMatrix(Matrix m)
{
    for (int i = 0; i < m.rows; i++)
    {
        cout << "[";
        for (int j = 0; j < m.cols; j++)
        {
            if (j + 1 < m.cols)
                cout << m.data[i][j] << " ";
            else
                cout << m.data[i][j];
        }
        cout << "]" << endl;
    }
}

int main()
{

    int rows = 3;
    int cols = 3;

    Matrix m1 = Matrix(rows, cols);
    Matrix m2 = Matrix(rows, cols);

    // fill matrixes starting from num 1,2,3..col
    int num = 1;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            m1.data[i][j] = num;
            m2.data[i][j] = num;
            num++;
        }
    }
    cout << "Matrix 1: " << endl;
    printMatrix(m1);
    cout << "Matrix 2: " << endl;
    printMatrix(m2);

    cout << "Multiplying Matrix 1 with Matrix 2: " << endl;
    Matrix C = dotProduct(m1, m2);
    printMatrix(C);

    return 0;
}