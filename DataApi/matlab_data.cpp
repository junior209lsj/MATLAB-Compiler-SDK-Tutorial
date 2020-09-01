// Author: junior209@naver.com
// This code is example of MATLAB Data API
#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "MatlabCppSharedLib.hpp"

// 1차원 인덱스를 row major 2차원 인덱스로 변환
#define IDX2C(i, j, len) (((i)*(len))+(j))

void AllocateMap(std::map<int, std::string> &m);

int main(void)
{
    std::map<int, std::string> cppType;
    AllocateMap(cppType);
    
    const size_t N_ROWS= 2;
    const size_t N_COLS= 5;
    // factory: Matlab 호환 배열을 만들기 위한 클래스
    // TypedArray: 템플릿으로 타입을 정할 수 있는 Matlab 호환 Array
    matlab::data::ArrayFactory factory;
    // 이렇게 할당하면
    // 1 3 5 7 9
    // 2 4 6 8 10 으로 할당됨
    matlab::data::TypedArray<double> A = factory.createArray<double>({N_ROWS, N_COLS}, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    //getDimensions() 배열 크기 반환
    //getType() 데이터타입 반환
    //getMemoryLayout() Row major인지 Column major인지 반환
    matlab::data::ArrayDimensions dims = A.getDimensions();
    matlab::data::ArrayType arrType = A.getType();
    matlab::data::MemoryLayout memLayout = A.getMemoryLayout();

    std::string AType = cppType.find(static_cast<int>(arrType))->second;

    std::cout << "Data type of A: " << AType << std::endl;

    std::cout << "Memory layout of A: ";
    switch(static_cast<int>(memLayout))
    {
        case static_cast<int>(matlab::data::MemoryLayout::COLUMN_MAJOR):
        {
            std::cout << "Column major" << std::endl;
            break;
        }
        case static_cast<int>(matlab::data::MemoryLayout::ROW_MAJOR):
        {
            std::cout << "Row major" << std::endl;
            break;
        }
        default:
        {
            break;
        }
    }

    size_t nDim = dims.size();
    std::cout << "Dimension of A: " << nDim << std::endl;

    std::cout << "Size of A: ";
    {
        std::cout << "(";
        for (size_t i = 0; i < nDim; i++)
        {
            std::cout << dims[i];
            if (i != nDim - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ")" << std::endl;
    }

    std::cout << "Elements of A" << std::endl;
    {
        std::cout << std::endl;
        for(size_t i = 0; i < dims[0]; i++)
        {
            for(size_t j = 0; j < dims[1]; j++)
            {
                std::cout << A[i][j] << "   ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    matlab::data::TypedArray<double> B = factory.createArray<double>({N_ROWS, N_COLS}, {2, 4, 6, 8, 10, 1, 3, 5, 7, 9});

    std::cout << "Elements of B" << std::endl;
    {
        std::cout << std::endl;
        for (size_t i = 0; i < N_ROWS; i++)
        {
            for (size_t j = 0; j < N_COLS; j++)
            {
                std::cout << B[i][j] << "   ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    // 데이터 간 덧셈 구현

    // 0으로 초기화된 배열 만들기
    matlab::data::TypedArray<double> res = factory.createArray<double>({N_ROWS, N_COLS});

    std::cout << "A + B" << std::endl;
    {
        std::cout << std::endl;
        for (size_t i = 0; i < N_ROWS; i++)
        {
            for (size_t j = 0; j < N_COLS; j++)
            {
                res[i][j] = A[i][j] + B[i][j];
                std::cout << res[i][j] << "   ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }

    //결과를 C++ 1차원 벡터로 이동하기
    std::vector<double> result(N_ROWS*N_COLS);
    // Column major 순서로 기록됨
    {
        int i = 0;
        for (auto e : res)
        {
            result[i++] = e;
        }
    }
    // Row major 순서로 제대로 기록하려면
    // {
    //     for (int i = 0; i < N_ROWS; i++)
    //     {
    //         for (int j = 0; j < N_COLS; j++)
    //         {
    //             result[IDX2C(i, j, N_COLS)] = res[i][j];
    //         }
    //     }
    // }

    std::cout << "copied vector result" << std::endl;
    {
        std::cout << std::endl;
        for (size_t i = 0; i < N_ROWS; i++)
        {
            for (size_t j = 0; j < N_COLS; j++)
            {
                std::cout << result[IDX2C(i, j, N_COLS)] << "   ";
            }
            std::cout << std::endl;
            std::cout << std::endl;
        }
    }
}

// getType의 결과와 그에 대응하는 C++ 타입을 map에 매칭
void AllocateMap(std::map<int, std::string> &m)
{
    m.insert(std::make_pair(0, "Unknown"));
    m.insert(std::make_pair(1, "bool"));
    m.insert(std::make_pair(2, "char16_t"));
    m.insert(std::make_pair(3, "double"));
    m.insert(std::make_pair(4, "float"));
    m.insert(std::make_pair(5, "int8_t"));
    m.insert(std::make_pair(6, "uint8_t"));
    m.insert(std::make_pair(7, "int16_t"));
    m.insert(std::make_pair(8, "uint16_t"));
    m.insert(std::make_pair(9, "int32_t"));
    m.insert(std::make_pair(10, "uint32_t"));
    m.insert(std::make_pair(11, "int64_t"));
    m.insert(std::make_pair(12, "uint64_t"));
    m.insert(std::make_pair(13, "std::complex<double>"));
    m.insert(std::make_pair(14, "std::complex<float>"));
    m.insert(std::make_pair(15, "std::complex<int8_t>"));
    m.insert(std::make_pair(16, "std::complex<uint8_t>"));
    m.insert(std::make_pair(17, "std::complex<int16_t>"));
    m.insert(std::make_pair(18, "std::complex<uint16_t>"));
    m.insert(std::make_pair(19, "std::complex<int32_t>"));
    m.insert(std::make_pair(20, "std::complex<uint32_t>"));
    m.insert(std::make_pair(21, "std::complex<int64_t>"));
    m.insert(std::make_pair(22, "std::complex<uint64_t>"));
    m.insert(std::make_pair(23, "matlab::data::Array"));
    m.insert(std::make_pair(24, "matlab::data::Struct"));
    m.insert(std::make_pair(25, "matlab::data::Object"));
    m.insert(std::make_pair(26, "matlab::data::Object"));
    m.insert(std::make_pair(27, "matlab::data::Enumeration"));
    m.insert(std::make_pair(28, "bool"));
    m.insert(std::make_pair(29, "double"));
    m.insert(std::make_pair(30, "std::complex<double>"));
    m.insert(std::make_pair(31, "matlab::data::MATLABString"));

    return;
}