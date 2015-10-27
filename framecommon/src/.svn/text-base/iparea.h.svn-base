/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-12-30
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __IP_AREA_H
#define __IP_AREA_H
#include <string>
#include <map>
using namespace std;
namespace MYFRAMECOMMON{

const int PROVINCENUM=10000;

struct TIPArea
{
        unsigned int nBeginIp;
        unsigned int nEndIp;
        int nArea;
};

/**
* dec:
* ClassName:CIpAreaInfo
* author: 
* version:
*CreateTime: 2006.12.30
*/
class CIpAreaInfo
{
public:
        CIpAreaInfo();
        ~CIpAreaInfo();
        int LoadIPArea(string sHost, string sUser, string sPasswd, string sDbName);
        int LoadIPAreaFromFile(string sFileName);
        TIPArea &GetIpArea(unsigned int nIp);
private:
        map<unsigned int, TIPArea> m_ipAreaList;
        map<string, int> m_areaList;
        TIPArea m_defautArea;
};
}

#endif
