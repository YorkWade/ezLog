#ifndef EZ_LOGFILE_H
#define EZ_LOGFILE_H
#include "NoCopyable.h"
#include "Mutex.h"

#include <memory>
#include <string>
#include <time.h>

namespace Ez
{

class AppendFile;

class LogFile:public NoCopyable
{
public:
	LogFile(const std::string& basename,
		off_t rollSize,
		bool threadSafe = true,
		int flushInterval = 3,
		int checkEveryN = 1024);
	~LogFile();

	void append(const char* logline, int len);
	void flush();
	bool rollFile();

private:
	void append_unlocked(const char* logline, int len);

	static std::string getLogFileName(const std::string& basename, time_t* now);

	const std::string basename_;
	const off_t rollSize_;
	const int flushInterval_;
	const int checkEveryN_;

	int count_;

	std::unique_ptr<MutexLock> mutex_;
	time_t startOfPeriod_;
	time_t lastRoll_;
	time_t lastFlush_;
	std::unique_ptr<AppendFile> file_;

	const static int kRollPerSeconds_ = 60*60*24;
};


}


#endif