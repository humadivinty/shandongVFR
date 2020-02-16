#ifndef H264_H
#define H264_H

#ifdef H264_EXPORTS
#define H264API extern "C" __declspec(dllexport)
#else
#define H264API extern "C" __declspec(dllimport)
#endif

// 打开rtsp视频（rtsp://xxx.xxx.xxx.xxx/h264ESVideoTest）
H264API HANDLE WINAPI H264_Play(HWND hWnd, LPCSTR szURL);

// 关闭rtsp视频
H264API VOID WINAPI H264_Destroy(HANDLE hPlay);

// 设置退出，多对象时可减少等待
H264API VOID WINAPI H264_SetExitStatus(HANDLE hPlay);
H264API bool WINAPI H264_GetOneBmpImg(HANDLE hPlay, PBYTE DestImgData, int& iLength, int& iWidth, int& iHeight);
#endif // H264_H
