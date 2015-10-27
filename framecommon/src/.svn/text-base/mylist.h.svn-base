/*
 * FileName:       
 * Author:         wangqifeng  Version: 1.0  Date: 2009-3-9
 * Description:    
 * Version:        
 * Function List:  
 *                 1.
 * History:        
 *     <author>   <time>    <version >   <desc>
 */
#ifndef __MY_LIST_H
#define __MY_LIST_H
#include <list>
using namespace std;
template<class T>
class CMyList:protected list<T>
{
    public:
        typedef _List_iterator<T>    iterator;
        CMyList()
        {
            m_nNum=0;
        }
        void push_back(const T& __x)
        {
            m_nNum++;
            list<T>::push_back(__x);
        }
        iterator erase(iterator itr)
        {
            m_nNum--;
            return list<T>::erase(itr);
        }

        iterator begin()
        {
            return list<T>::begin();
        }

        iterator end()
        {
            return list<T>::end();
        }

        int GetNum()
        {
            return m_nNum;
        }
    protected:
        int m_nNum;
};
#endif

