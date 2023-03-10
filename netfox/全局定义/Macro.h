#ifndef MACRO_HEAD_FILE
#define MACRO_HEAD_FILE

#include <random>
#include <time.h>

//随机类型
class CRandRange 
{
public:
	CRandRange() :gen((unsigned int)time(NULL)) {}
	~CRandRange() {}

	int Range()
	{
		std::uniform_int_distribution<unsigned> u(1, RAND_MAX);
		std::default_random_engine e;
		e.seed((unsigned)time(NULL));
		return u(e);
	}

	int Range(int min, int max) {
		if (min > max) min = max;
		std::uniform_int_distribution<> dis(min, max);
		return dis(gen);
	}


	double Range(double min, double max) {
		if (min > max) min = max - 0.000001;
		std::uniform_real_distribution<> dis(min, max);
		return dis(gen);
	}

	LONGLONG Range(LONGLONG min, LONGLONG max) {
		return (LONGLONG)Range(double(min), double(max));
	}

private:
	std::mt19937 gen;
};

//全局对象
static CRandRange  g_RandRange;

//////////////////////////////////////////////////////////////////////////////////

//常用常量
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////////////

//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//整数随机
#define RAND_VALUE  g_RandRange.Range()

//范围随机
#define RAND_RANGE(min, max)  g_RandRange.Range(min, max)

//范围随机
#define RAND_RANGE_VER2(min, max) g_RandRange.Range(min, max)

//区间验证
#define FAST_ODDS(x) ((rand()%100)<(x))

//四舍五入
#define Round(X)   ((X-floor(X)) >=0.5) ? ceil(X): floor(X)

//////////////////////////////////////////////////////////////////////////////////

//存储长度
#ifdef _UNICODE
	#define CountStringBuffer CountStringBufferW
#else
	#define CountStringBuffer CountStringBufferA
#endif

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

//////////////////////////////////////////////////////////////////////////////////

//接口释放
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//删除指针
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//关闭句柄
#define SafeCloseHandle(hHandle) { if (hHandle!=NULL) { CloseHandle(hHandle); hHandle=NULL; } }

//删除数组
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//////////////////////////////////////////////////////////////////////////////////

#endif