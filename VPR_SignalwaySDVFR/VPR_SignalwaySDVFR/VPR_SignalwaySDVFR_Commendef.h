#ifndef VPR_COMMENTDEF_H
#define VPR_COMMENTDEF_H
/*
注：
1、	图片存储目录为：动态库所在目录，. / idevlp / xxxx.jpg
2、	图片格式：JPEG，扩展名：JPG
3、	视频格式：MPEG - 4，扩展名：MP4
4、	各图片文件名命名规则：
车头场景图文件名：. / idevlp / vlp_0_xx.jpg
车头车牌图文件名：. / idevlp / vlp_1_xx.jpg
车头二值化图文件名：. / idevlp / vlp_2_xx.jpg
车身图文件名：. / idevlp / vlp_3_xx.jpg
车尾场景图文件名：. / idevlp / vlp_4_xx.jpg
车尾车牌图文件名：. / idevlp / vlp_5_xx.jpg
车尾二值化图文件名：. / idevlp / vlp_6_xx.jpg
五秒视频文件名：. / idevlp / vlp_7_xx.jpg
其中：XX为00 - 99循环使用，每过一辆车加1，到99后从00重新开始。

5.13	车牌颜色码
值	描述
00	蓝色
01	黄色
02	黑色
03	白色
04	渐变绿色
05	黄绿双拼色
06	蓝白渐变色
09	未知

5.14	车牌识别接口错误码
错误码	错误描述
0	正常
-100	设备无响应
-1000	传入参数错误
-1001	设备被占用
-1002	打开失败
-2000	其他错误
-2000以上	预留

*/


//typedef struct _vlp_info
//{
//	int vlpInfoSize;					//识别结构体大小
//	unsigned char vlpColor[2];			//车头车牌颜色(编码详见5.14);
//	unsigned char vlpText[16];			//车头车牌文字，GBK编码
//	char vlpTime[20];          		//车头识别时间，格式“yyyyMMddHHmmsszzz”
//	unsigned char vlpBackColor[2];		//车尾车牌颜色(编码详见5.14);
//	unsigned char vlpBackText[16];		//车尾车牌文字，GBK编码
//	char vlpBackTime[20];          	//车尾识别时间，格式“yyyyMMddHHmmsszzz”
//	int vlpCarClass;					//车型
//	int vehLength;					//识别车长
//	int vehWidth;						//识别车宽
//	int vehHigh;						//识别车高
//	int vehAxis;						//识别轴数
//	unsigned int vlpReliability;			//可信度(采用四位表示9999表示为99.99%);
//	unsigned char imageFile[8][128];		//识别图片长度:[0]=车头场景图文件名，[1]=车头车牌图文件名，[2]=车头二值化图文件名，[3]=车身图文件名，[4]=车尾场景图文件名，[5]=车尾车牌图文件名，[6]=车尾二值化图文件名，[7]=五秒视频文件名
//} T_VLPINFO;

//typedef void (*CBFun_GetRegResult)(int nHandle,T_VLPINFO* pVlpResult, void *pUser);


typedef struct _vlp_front_info
{
	int vlpInfoSize;					//识别结构体大小
	unsigned char vlpColor[2];			//车头车牌颜色(编码详见5.14)
	unsigned char vlpText[16];			//车头车牌文字，GBK编码
	char vlpTime[20];          		//车头识别时间，格式“yyyyMMddHHmmsszzz”
	unsigned int vlpReliability;			//可信度(采用四位表示9999表示为99.99%)
	unsigned char imageFile[3][128];		//识别图片长度:[0]=车头场景图文件名，[1]=车头车牌图文件名，[2]=车头二值化图文件名
} T_VLPFRONTINFO;


typedef struct _vlp_back_info
{
	int vlpInfoSize;					//识别结构体大小
	unsigned char vlpBackColor[2];		//车尾车牌颜色(编码详见5.14)
	unsigned char vlpBackText[16];		//车尾车牌文字，GBK编码
	char vlpBackTime[20];          	//车尾识别时间，格式“yyyyMMddHHmmsszzz”
	int vlpCarClass;					//车型
	int vehLength;					//识别车长
	int vehWidth;						//识别车宽
	int vehHigh;						//识别车高
	int vehAxis;						//识别轴数
	unsigned int vlpReliability;			//可信度(采用四位表示9999表示为99.99%)
	unsigned char imageFile[5][128];		//识别图片长度: [0]=车身图文件名，[1]=车尾场景图文件名，[2]=车尾车牌图文件名，[3]=车尾二值化图文件名，[4]=五秒视频文件名
} T_VLPBACKINFO;


typedef void(*CBFun_GetFrontResult)(
	int nHandle,
	T_VLPFRONTINFO* pVlpResult,
	void *pUser
	);


typedef void(*CBFun_GetBackResult)(
	int nHandle,
	T_VLPBACKINFO* pVlpResult,
	void *pUser
	);


typedef void(*CBFun_GetDevStatus)(	int nHandle,	int nStatus,	void* pUser	);
#endif
