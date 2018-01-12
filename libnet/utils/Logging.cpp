
#include "Logging.hpp"
#include "LogStream.hpp"
#include "TimeStamp.hpp"
#include "CurrentThread.hpp"


namespace logg
{
LogLevel initLogLevel()
{
  if (::getenv("LOG_TRACE"))
    return LogLevel::TRACE;
  else if (::getenv("LOG_DEBUG"))
    return LogLevel::DEBUG;
  else
    return LogLevel::INFO;
}

__thread char t_errnobuf[512] = {0};
const char* strerror_tl(int savedErrno)
{
  return strerror_r(savedErrno, t_errnobuf, sizeof(t_errnobuf));
}

LogLevel g_logLevel =  initLogLevel();
const std::string g_logLevelName[] = {"TRACE ","DEBUG ","INFO  ","WARN  ","ERROR ","FATAL "};

__thread time_t g_lastLogSecond = {0};
__thread char g_Logtime[32] = {0};
}



class LoggerImpl
{
friend class Logger;

private:
LoggerImpl(LogLevel level, const char* fileName, int lineNum):
time_(TimeStamp::now()),
level_(level),
lineNum_(lineNum),
srcfile(fileName),
stream_()
{
  formatTime();
  currentThread::tid();
  stream_ << std::string(currentThread::tidString(), static_cast<size_t>(currentThread::tidStringLength()));
  stream_<<logg::g_logLevelName[static_cast<int>(level)];
}

~LoggerImpl() = default;

void finish()
{
  stream_ << " - " << srcfile.name << ':' << lineNum_ << '\n';
  const LogStream::LogBuffer& buf(stream_.buffer());
  fwrite(buf.data(), sizeof(char), buf.length(), stdout);
  if(level_ == LogLevel::FATAL)
  {
      fflush(stdout);
      abort();
  }
}

void formatTime()
{
  auto sms = time_.get();
  time_t seconds = static_cast<time_t>(sms.first);
  if (seconds != logg::g_lastLogSecond)
  {
    logg::g_lastLogSecond = seconds;
    time_.formatSeconds(logg::g_Logtime);
  }
  char timeMs[32];
  time_.formatMs(timeMs);
  
  stream_ << static_cast<const char*>(logg::g_Logtime) << static_cast<const char*>(timeMs);
}
  
private:
  class FileNameExtractor
  {
  public:
    explicit FileNameExtractor(const char* filenameWithPath) 
    {
      const char* slash = strrchr(filenameWithPath, '/');
      const char* data = slash ? (slash + 1) : filenameWithPath;
      auto len = static_cast<size_t>(strlen(data));
      name = std::string(data, len);
    }
    std::string name;
  };

private:
  TimeStamp time_;
  LogLevel level_;
  int lineNum_;
  FileNameExtractor srcfile;
  LogStream stream_;

};




Logger::Logger(const char* fileName, int lineNum, const char* func, LogLevel level):
  impl_(new LoggerImpl(level, fileName, lineNum))
{
    impl_->stream_ << func << ' ';
}

Logger::~Logger()//output log in dtor
{
  impl_->finish();
  delete impl_;
}

LogStream& Logger:: stream()
{
  return impl_->stream_;
}

