#include <string>
#include <iostream>
#include "regex.h"
using namespace std;
#ifndef __SKTOKEN__
#define __SKTOKEN__

class SKToken
{
    protected:
        string m_matchvalue;
        string m_name;
        string m_pattern;
        Regex m_expression;

    public:
        SKToken *prev;
        SKToken *next;

    private:
        SKToken()
        {
            prev = NULL;
            next = NULL;
        }

    public:
        virtual ~SKToken() {}
        SKToken(string name, const char *expression) 
        {
            m_pattern = expression;
            m_name = name;
            m_expression.SetPattern(expression);
            prev = NULL;
            next = NULL;
        }

        SKToken *Copy()
        {
            SKToken *returnvalue = new SKToken();
            returnvalue->m_name = m_name;
            returnvalue->m_expression.SetPattern(m_pattern.c_str());
            returnvalue->m_pattern = m_pattern;
            returnvalue->m_matchvalue = m_matchvalue;
            return returnvalue;
        }

        bool Match(char *value)
        {
            bool returnvalue = false;
            m_matchvalue = m_expression.Match(value);
            if(!m_matchvalue.empty())
            {
                returnvalue = true;
            }
            return returnvalue;
        }
        string Name() {return m_name;}
        string MatchedValue() { return m_matchvalue;}
        string GetToken()
        {
            string returnvalue = m_name + "[" + m_matchvalue + "] ";
            return returnvalue;
        }
};
#endif
