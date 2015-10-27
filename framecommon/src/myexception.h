/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2006-11-25
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __MY_EXCEPTION_H
#define  __MY_EXCEPTION_H
#include <string>
using namespace std;
namespace MYFRAMECOMMON{

/**
* dec:
* ClassName:CMyException
* author: 
* version:
*CreateTime: 2006.11.25
*/
class CMyException
{
public:
        CMyException(string sErrMsg)
        {
                m_sErrMsg = sErrMsg;
        }
        ~CMyException()
        {
                
        }
        
        string &GetErrMsg()
        {
                return m_sErrMsg;
        }
protected:
        string m_sErrMsg;
};
}
#endif

