#if !defined(Array2D_H)
#define Array2D_H

#include <stdlib.h>
#include <stdio.h>

template <typename Type>
class Array2D
{
public:
    Array2D() {}

    Array2D(int cols, int rows)
    {
        initArray2D(cols, rows);
    }

    void initArray2D(int cols, int rows)
    {
        mCols = cols;
        mRows = rows;
        if (mData)
        {
            free(mData);
        }

        mData = (Type *)malloc(cols * rows * sizeof(Type));
    }

    void initArray2D(int cols, int rows, Type initVal)
    {
        initArray2D(cols, rows);

        for (int i = 0; i < cols * rows; i++)
        {
            mData[i] = initVal;
        }
    }

    void initArray2D(int cols, int rows, void *pData)
    {
        mCols = cols;
        mRows = rows;

        if (mData)
        {
            free(mData);
        }

        mData = (Type *)pData;
    }

    ~Array2D()
    {
        destroy();
    }

    void destroy()
    {
        if (mData)
        {
            free(mData);
            mData = NULL;
        }
    }

    Type *getAddr(int col, int row) const
    {
        size_t index = calcIndex(col, row);
        return &mData[index];
    }

    Type *getBaseAddr() const
    {
        return mData;
    }

    int getSize() const
    {
        return mCols * mRows;
    }

    int getSizeInBytes() const
    {
        return getSize() * sizeof(Type);
    }

    const Type &get(int col, int row) const
    {
        return *getAddr(col, row);
    }

    const Type &get(int index) const
    {
#ifndef NDEBUG
        if (index >= mRows * mCols)
        {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, index, mRows * mCols);
            exit(0);
        }
#endif

        return m_p[index];
    }

    Type &at(int col, int row)
    {
        size_t index = calcIndex(col, row);
        return mData[index];
    }

    void set(int col, int row, const Type &val)
    {
        *getAddr(col, row) = val;
    }

    void Set(int index, const Type &Val)
    {
#ifndef NDEBUG
        if (index >= mRows * mCols)
        {
            printf("%s:%d - index %d is out of bounds (max size %d)\n", __FILE__, __LINE__, index, mRows * mCols);
            exit(0);
        }
#endif

        m_p[index] = Val;
    }

    void getMinMax(Type &min, Type &max)
    {
        max = min = mData[0];

        for (int i = 1; i < mCols * mRows; i++)
        {
            if (mData[i] < min)
            {
                min = mData[i];
            }

            if (mData[i] > max)
            {
                max = mData[i];
            }
        }
    }

    void normalize(Type minRange, Type maxRange)
    {
        Type min, max;

        getMinMax(min, max);

        if (max <= min)
        {
            return;
        }

        Type minMaxDelta = max - min;
        Type minMaxRange = maxRange - minRange;

        for (int i = 0; i < mRows * mCols; i++)
        {
            mData[i] = (mData[i] - min) / minMaxDelta * minMaxRange + minRange;
        }
    }

    void printFloat()
    {
        for (int y = 0; y < mRows; y++)
        {
            printf("%d: ", y);
            for (int x = 0; x < mCols; x++)
            {
                float f = (float)mData[y * mCols + x];
                printf("%.6f ", f);
            }
            printf("\n");
        }
    }

private:
    size_t calcIndex(int col, int row) const
    {
#ifndef NDEBUG
        if (col < 0)
        {
            printf("%s:%d - negative col %d\n", __FILE__, __LINE__, col);
        }

        if (col >= mCols)
        {
            printf("%s:%d - column overflow (%d vs %d)\n", __FILE__, __LINE__, col, mCols);
            exit(0);
        }

        if (row < 0)
        {
            printf("%s:%d - negative row %d\n", __FILE__, __LINE__, row);
        }

        if (row >= mRows)
        {
            printf("%s:%d - row overflow (%d vs %d)\n", __FILE__, __LINE__, row, mRows);
            exit(0);
        }
#endif
        size_t index = row * mCols + col;

        return index;
    }

    Type *mData = nullptr;
    int mCols = 0;
    int mRows = 0;
};

#endif // Array2D_H
