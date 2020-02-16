#include "stdafx.h"
#include "MyH264Vector.h"

MyH264DataVector::MyH264DataVector(int maxSize):
m_iInsertIndex(0),
bHasLoopAround(false)
{
    InitializeCriticalSection(&m_hcs);
    m_list.resize(maxSize);

}
 
MyH264DataVector::~MyH264DataVector()
{
    DeleteCriticalSection(&m_hcs);
}

void MyH264DataVector::AddOneData(Result_Type h264Data)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if (m_iInsertIndex == m_list.capacity())
    {
        m_iInsertIndex = 0;
        bHasLoopAround = true;
    }
    m_list[m_iInsertIndex++] = h264Data;
}

MyH264DataVector::Result_Type MyH264DataVector::GetOneDataByIndex(int index)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if (index < 0 || index >= m_list.capacity())
    {
        return nullptr;
    }
    return m_list[index];
}

int MyH264DataVector::GetOldestDataIndex()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    if (!bHasLoopAround)
    {
        return 0;
    }
    else
    {
        if (m_iInsertIndex >= m_list.capacity() -10)
        {
            return 0;
        }
        else
        {
            return m_iInsertIndex + 10;
        }        
    }
}
