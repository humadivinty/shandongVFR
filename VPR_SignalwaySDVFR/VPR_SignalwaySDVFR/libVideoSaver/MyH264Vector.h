#ifndef MY_H264_VECTOR
#define  MY_H264_VECTOR
#include "libVideoSaver/CusH264Struct.h"
//#include "MyLockerDef.h"
#include<vector>
#include<windows.h>
#include <memory>
#include <mutex>

class MyH264DataVector
{
    typedef std::shared_ptr<CustH264Struct> Result_Type;
    typedef std::vector<Result_Type> list_Type;
public:
    MyH264DataVector(int maxSize);
    ~MyH264DataVector();

    void AddOneData(Result_Type data);
    Result_Type GetOneDataByIndex(int index);
    int GetOldestDataIndex();

private:
    int m_iInsertIndex;
    bool bHasLoopAround;        //当前数据是否循环了一圈
    list_Type m_list;
    std::mutex m_mtx;
    CRITICAL_SECTION m_hcs;

    //friend class MYLocker;
};



#endif // !MY_H264_VECTOR
