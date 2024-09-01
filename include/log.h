#include <string>
#include <iostream>
#include <chrono>
using namespace std;

enum class SKLogType
{
    None = 0,
    Basic = 1,
    Advanced = 2,
    Verbose = 3,
    Debug = 4
};

class SKLog
{
    public: 
    static chrono::high_resolution_clock::time_point _lastTime;

    static SKLogType _level;
    public:
        static void SetLevel(SKLogType level)
        {
            _level = level;
            _lastTime = chrono::high_resolution_clock::now();
        }
        static SKLogType GetLevel()
        {
            return _level;
        }
        static void Log(string log, SKLogType type)
        {
            if(type <= _level)
            {
                string intro;
                switch(type)
                {
                    case SKLogType::Basic:
                        intro = "BASIC: ";
                        break;
                    case SKLogType::Advanced:
                        intro = "ADVANCED: ";
                        break;
                    case SKLogType::Verbose:
                        intro = "VERBOSE: ";
                        break;
                    case SKLogType::Debug:
                        intro = "DEBUG: ";
                        break;
                    case SKLogType::None:
                        break;
                }
                cout << intro << log << endl;
            }
        }
        static void LogTime(string message, SKLogType type)
        {
            auto newTime = chrono::high_resolution_clock::now();
            auto ms = chrono::duration_cast<std::chrono::milliseconds>(newTime-SKLog::_lastTime);
            if(message != "") SKLog::Log(to_string(ms.count())+string(": ")+message, type);
            SKLog::_lastTime = newTime;
        }
};