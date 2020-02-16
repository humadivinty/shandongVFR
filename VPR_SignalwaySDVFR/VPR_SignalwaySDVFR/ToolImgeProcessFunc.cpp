#include "stdafx.h"
#include "ToolImgeProcessFunc.h"
#include "libjpeg/turbojpeg.h"
#include <io.h>
#include <stdio.h>
#include <string.h>
#include "utilityTool/log4z.h"
//#pragma comment(lib, "libjpeg/turbojpeg.lib")
#pragma comment(lib, "libjpeg/turbojpeg-static.lib")


int Tool_CompressImg(unsigned char* srcImgData, size_t srcImgSize, unsigned char* destImgBuffer, size_t& bufLength, size_t wantSize)
{
    if (NULL == srcImgData
        || NULL == destImgBuffer
        || bufLength <= 0
        || srcImgSize <= wantSize)
    {
        return -1;
    }

    int iRet = -1;
    tjhandle hProcess = tjInitCompress();
    if (hProcess)
    {
        unsigned char *jpegBuf = srcImgData;
        long jpegSize = srcImgSize;

        int _width = 0;
        int _height = 0;
        int jpegSubsamp = 0;
        int jpegColorspace = 0;
        int pad = 2;
        unsigned char* rgbbuf = nullptr;

        // 获取jpeg文件信息
        tjhandle dh = tjInitDecompress();
        int rel = tjDecompressHeader3(dh, jpegBuf, jpegSize, &_width, &_height, &jpegSubsamp, &jpegColorspace);

        // 计算YUV buffer大小
        unsigned long yuvBufSize = tjBufSizeYUV2(_width, pad, _height, jpegSubsamp);
        rgbbuf = tjAlloc(yuvBufSize);

        // 获取YUV buffer
        rel = tjDecompressToYUV2(dh, jpegBuf, jpegSize, rgbbuf, _width, pad, _height, 0);
        if (rel != 0)
        {
            char* err = tjGetErrorStr();
            LOGFMTD("tjDecompressToYUV2 failed, msg = %s\n", err);
        }
        rel = tjDestroy(dh);
        //LOGFMTD("convert to yuv image data length = %ld\n", yuvBufSize);

        // 分配目的buffer
        unsigned long dstSizes = 0;
        unsigned char* dstBufs = 0;
        int quality = 20;
        bool bCompressSuccess = false;
        do
        {
            dstSizes = 0;
            rel = tjCompressFromYUV(hProcess, rgbbuf, _width, pad, _height, jpegSubsamp, &dstBufs, &dstSizes, quality, 0);
            //LOGFMTD("compress  yuv to jpeg image data length = %ld\n", dstSizes);
            quality = quality - 5;

            
            if (dstSizes > wantSize)
            {
                if (dstSizes != 0)
                {
                    tjFree(dstBufs);
                    dstBufs = 0;
                    //dstSizes = 0;
                }
            }
            else
            {
                bCompressSuccess = true;
                break;
            }
        } while (quality > 5);
        // 压缩
        

        if (bCompressSuccess)
        {
            memcpy(destImgBuffer, dstBufs, dstSizes);
            bufLength = dstSizes;
            iRet = 0;
        }
        else
        {
            LOGFMTD("orinal size = %d, final quality = %d, size = %d , but still not match want size %d", srcImgSize, quality, dstSizes, wantSize);
            bufLength = 0;
        }

        // 释放buffer
        if (dstSizes != 0)
            tjFree(dstBufs);
        tjFree(rgbbuf);
        //free(jpegBuf);

        // Destroy a TurboJPEG compressor instance
        tjDestroy(hProcess);
    }
    return iRet;
}
