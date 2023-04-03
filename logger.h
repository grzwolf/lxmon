#ifndef LOGGER_H
#define LOGGER_H


#include <time.h>
#include <unistd.h>

// based on: https://stackoverflow.com/questions/7411301/how-to-introduce-date-and-time-in-log-file
class LogHelper {
    inline static char timeStr[20];
    inline static char dayStr[20];
    inline static char fileNameBase[255];
    inline static char fullFileName[280];

        public:
        static char* getFormattedTime(void)
        {
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
            return timeStr;
        }
        static char* getFormattedFilename(void)
        {
            time_t rawtime;
            struct tm* timeinfo;
            time(&rawtime);
            timeinfo = localtime(&rawtime);
            strftime(dayStr, sizeof(dayStr), "_%Y-%m-%d.log", timeinfo);
            sprintf(fileNameBase, "%s", QCoreApplication::applicationFilePath().toStdString().c_str());
            sprintf(fullFileName, "%s%s", fileNameBase, dayStr);
            return fullFileName;
        }
};
// remove path from filename
#define __SHORT_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
// main log macro
#define __LOG__(format, loglevel, ...) {                                                                                                      \
                static char logTime[21];                                                                                                      \
                strncpy(logTime, LogHelper::getFormattedTime(), 20);                                                                          \
                fprintf(stderr, "%s %-5s [%s] [%s:%d] " format "\n", logTime, loglevel, __func__, __SHORT_FILE__, __LINE__, ## __VA_ARGS__);  \
                FILE* file = fopen(LogHelper::getFormattedFilename(), "a");                                                                   \
                fprintf(file, "%s %-5s [%s] [%s:%d] " format "\n", logTime, loglevel, __func__, __SHORT_FILE__, __LINE__, ## __VA_ARGS__);    \
                fflush(file);                                                                                                                 \
                fclose(file);                                                                                                                 \
                sync();                                                                                                                       \
}
// specific log macros with arguments
#define LOGINFO(format, ...) __LOG__(format, "INFO", ## __VA_ARGS__)
#define LOGERROR(format, ...) __LOG__(format, "ERROR", ## __VA_ARGS__)


#endif // LOGGER_H
