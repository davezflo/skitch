#include "log.h"

SKLogType SKLog::_level = SKLogType::None;
chrono::high_resolution_clock::time_point SKLog::_lastTime = chrono::high_resolution_clock::now();