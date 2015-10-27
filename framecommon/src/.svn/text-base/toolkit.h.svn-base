/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2005-11-1
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */ 
#ifndef  __TOOLKIT_H_
#define  __TOOLKIT_H_
#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <map>
#include "commondef.h"
using namespace std;
namespace MYFRAMECOMMON{

#define GBK_HIMIN       0x81                                                                   
#define GBK_HIMAX       0xfe                                                                   
#define GBK_LOMIN       0x40                                                                   
#define GBK_LOMAX       0xfe                                                                   
#define IN_RANGE(ch, min, max) ( (((unsigned char)(ch))>=min) && (((unsigned char)(ch))<=max) )

typedef unsigned long long ULONG64;

struct TMonth
{       
        int nMonth;
         char sMonth[10];
};

struct TWeek
{
        int nWeek;
        char sWeek[10];
};

//`Jan', `Feb', `Mar', `Apr', `May', `Jun', `Jul', `Aug',  `Sep',  `Oct',
//       `Nov',  and  `Dec'

const TMonth MONTHARRAY[]=
{
        {1, "Jan"},
        {2, "Feb"},
        {3, "Mar"},
        {4, "Apr"},
        {5, "May"},
        {6, "Jun"},
        {7, "Jul"},
        {8, "Aug"},
        {9, "Sep"},
        {10, "Oct"},
        {11, "Nov"},
        {12, "Dec"}
};

const TWeek WEEKARRAY[] = 
{
        {0, "Sun"},
        {1, "Mon"},
        {2, "Tue"},
        {3, "Wed"},
        {4, "Thu"},
        {5, "Fri"},
        {6, "Sat"}
};


const int REPLACESET[] =
{
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //0~15 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//16~31
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//32~47
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0,//48~63
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//64~79
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//80~95
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//96~111
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//112~127
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//128~143
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//144~150
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//160~175
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//176~191
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//1192~207
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//208~223
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//224~239
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0//240~255
};


//正则表达式转义字符集合
// \  * . [ ] | ( ) ?{}$^+:=
const int REGEXPESCAPE[] =
{
    //0   1   2   3   4   5   6   7   8   9   10 11 12 13 14  15
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //0~15 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//16~31
        0,  0,  0,  0,  1,  0,  0,  0,  1,  1,  1,  1,  0,  0,  1,  0,//32~47
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  1,  1,  1,  1,//48~63
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//64~79
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0,//80~95
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//96~111
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,//112~127
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//128~143
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//144~150
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//160~175
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//176~191
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//1192~207
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//208~223
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//224~239
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0//240~255
};

//url转义
// ?:/&|
const int URLESCAPE[] =
{
    //0   1   2   3   4   5   6   7   8   9   10 11 12 13 14  15
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, //0~15 
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//16~31
        0,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  1,//32~47
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  1,  0,  1,//48~63
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//64~79
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//80~95
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//96~111
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0,//112~127
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//128~143
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//144~150
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//160~175
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//176~191
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//1192~207
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//208~223
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,//224~239
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0//240~255
};

class  CToolKit
{
public:
        CToolKit(){};
        ~CToolKit(){};
        /**
         *生成uuid串
         *  \return   static string
         */
        static string  GenUuid();

        /**
         *获取随机字符串
         *  \param   &sStr
         *  \param   nLen
         *  \return   static  int
         */
        static  int GenRandomStr(string &sStr, int nLen);

        /**
         *字符串转换为整数
         *  \param   *pBuf
         *  \return   static  int
         */
        static  int Atoi(const char *pBuf);

        /**
         *转换字符串指针为float
         *  \param   *pBuf
         *  \return   static  float
         */
        static  float Atof(const char *pBuf);

        /**
         *转换字符串指针为string
         *  \param   *pBuf
         *  \return   static  string
         */
         
        static  string STRING(const char *pBuf);
        
        /**
         *字符串转换为64位整数
         *  \param   *pBuf
         *  \return   static  ULONG64
         */
        static  ULONG64 Atol64(const char *pBuf);

        /**
         *大写转换为小写 
         *  \param   &sStr
         *  \return   static  int
         */
        static  int CaptialToLow(string &sStr);
        
        /**
         *获取当前的0点秒数
         *  \param   nTime
         *  \return   static  int
         */
        static  int  GetCurZeroTime(time_t nTime);        
        
        /**
         *绝对秒转换为字符串显示的时间
         *  \param   lSecd
         *  \return   static  string
         */
        static  string Secd2Time(time_t lSecd);

        static string Secd2Time1(time_t lSecd);


        static  string Secd2Date(time_t lSecd);

        static  int DateToSec(const string &sDate); //YYYYMMDD

        static int DetailTimeToSec(const string & sTimeStr); //YYYYMMDDHHMMSS

        static time_t DateTimeToSec(const string &sDateTime); //YYYY-MM-DD HH:MM:SS
        static string SecToDateTime(const time_t nTime); // 返回YYYY-MM-DD HH:MM:SS

        
        /**
         *整数转换为字符串
         *  \param   nTmp
         *  \return   static  string
         */
        static  string IntToStr(int nTmp);
        
        /**
         *字符串中是否含有中文
         *  \param   *pStr
         *  \return   static  int
         */
        static  int IsHasChinese(char *pStr);
        
        /**
         *线程睡眠
         *  \param   nUsec
         *  \return   static  int
         */
        static  int USLEEP(int nUsec);

        /**
         *将assic码字符串转换为utf16格式
         *  \param   dest
         *  \param   nDestLen
         *  \param   src
         *  \param   nSrcLen
         *  \return   static void
         */
        static void String_Utf16(char* dest, int nDestLen,   char* src, int nSrcLen);

        static void StringTrim(string &sSrc);
        static unsigned int shortELFhash(const char* sStr, unsigned int nLen);


        /**
         *分割字符串， 按照delim的字符
         *  \param   &srcStr
         *  \param   &delim
         *  \param   strList
         *  \return   static int
         */
        static int SplitString(const string &srcStr,const string &delim, vector<string>& strList);

        /**
         *
         *  \param   *srcStr
         *  \param   &delim
         *  \param   strList
         *  \return   static int
         */
        static int SplitString(const char *srcStr, string &delim, vector<string>& strList);
        /**
         *读取目录下文件列表
         *  \param   &sPathdir
         *  \param   &fileList
         *  \return   static int
         */
        static int ReadDirfileList(const string &sPathdir,const string &sSuffix, vector<string> &fileList);

        static int GetUpdateNum(MYSQL *pMysql);

        static int ParseUrl(const string &sUrl, string &sIpAddr, int &nPort, string &sUrlFile, string &sHosts);

        static int ParseDomain(const string &sDomain, string &sIpAddr);

        //static int GetStringByLabel(string &sSrc, string sLabel1, string sLabel2, string &sOut, unsigned int nBeginPos=0);
        static int GetStringByLabel(const string &sSrc,const string &sLabel1,const string &sLabel2,
            string &sOut, unsigned int nBeginPos=0, int nIfRetPos=0);

        static int TimeToSec(const string &sDateStr); //Wed, 13 Dec 2006 03:10:20 GMT        

        static int GetMonthNum(const string &sMonth);
        static string GetWeekStr(int nWeek);
        static string GetMonStr(int nMonth);

        static string SecToHttpTime(time_t tmpTime);       

        static int DecodeUrl(const string &sSrc, string &sDest, int nIfTransChinese=1, int nIfTransPlus=0);
        static int UrlEncode(const string &sSrc,string &sDest);
        

        static int ChineseEncode(string &sSrc, string &sDest, int ifToUtf8=0);
        static int MyIconv(const string &sFromEncode,const string &sToEncode, string &sInput, string &sOut);

        static int EncodeSpace(string &sSRc);

        /**
         *转换sql语句中的特殊字符， 防止sql攻击
         *  \param   *sSrc
         *  \param   nLen
         *  \return   string
         */
        static string EscapeSqlStr(const char *sSrc, int nLen);
        static string EscapeSqlStr(const string &sSrc);

        static string EscapeJson(const string &sSrc);


        static string StrNewlinePro(const string &src);

        static string ToJsonStr(map<string, string> &mapStr, int ifEscape=1);

        static bool IsToday(const string &sTime);

        static string ReplaceStr(const string &sSrc);

        static string CutChinese(const string &sSrc);

        static int Html_Escape_Init();
        /**
         *
         *  \param   str
         *  \return   static string
         */
        static string Html_Escape(const string & str);
        /**
         *
         *  \param   &str
         *  \return   static string
         */
        static string LeiyanUnEscape(string &str);

        /**
         *
         *  \param   &str
         *  \return   string
         */
        static string RegExpEscape(string &str);

        static string GetFileCid(const string &sFileName);

		static string HexToBi(const char *pBuf, int nLen); 
		static string BiToHex(const char *pBuf, int nLen);

        static off_t GetFiveFileCid(const string &sFileName, string &sCid);

        template<class T1>
        static void DeleteContainerItem(T1 &t1)
        {   
            typename T1::iterator itr =  t1.begin();
            for (; itr != t1.end(); ++itr)
            {
                delete *itr;
            }
            return;
        }
		/*
		 *此函数的本意是为了将报文打印进trace
		 */
        static  void WriteBufToTrace(const char *pBuf, int nLen);
private:
        static int m_isInitRand;
        static map<unsigned char, string> m_specialCharList;
        static int m_nHtmlSpecial;
};
}
#endif

