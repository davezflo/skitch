#ifndef __PARSE_EXP__
#define __PARSE_EXP__

#include <exception>

class ParseException : public std::exception
{
private:
    string _reason;

public:
    ParseException(string reason)
    {
       _reason = reason; 
    }

    const char *what() const throw()
    {
        return _reason.c_str();
    }

};

class StateException : public std::exception
{
    private:
        string _reason;
    public:
        StateException(string reason)
        {
            _reason = reason; 
        }

        const char *what() const throw()
        {
            return _reason.c_str();
        }
};

class StateVarException : public std::exception
{
    private:
        string _reason;
    public:
        StateVarException(string reason)
        {
            _reason = reason; 
        }

        const char *what() const throw()
        {
            return _reason.c_str();
        }
};

class PointException : public std::exception
{
    private:
        string _reason;
    public:
        PointException(string reason)
        {
            _reason = reason;
        }

        const char *what() const throw()
        {
            return _reason.c_str();
        }
};

class ValueException : public std::exception
{
    private:
        string _reason;
    public:
        ValueException(string reason)
        {
            _reason = reason;
        }

        const char *what() const throw()
        {
            return _reason.c_str();
        }
};

#endif
