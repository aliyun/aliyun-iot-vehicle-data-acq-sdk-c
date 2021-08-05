#ifndef __LOG_H__
#define __LOG_H__ 

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif



typedef enum 
{
    LOG_STDOUT = 0,         //output to stdout only.
    LOG_FILE,               //output to file and stdout.
    LOG_DB,                 //output to db and stdout.
    LOG_FILE_DB             //output to db ,file and stdout.
} LOG_STORE_TYPE;

typedef enum 
{
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_MILESTONE
} LOG_LEVEL;

typedef enum {
    LOG_MOD_BRIEF_MIN = 0,  //min brief, only output to function name and log
    LOG_MOD_BRIEF,          //brief mode, default only output tag/function name/function line and log 
    LOG_MOD_VERBOSE         //verbos mode, output full log
} LOG_MODE;

int  log_init(const char *name, LOG_STORE_TYPE type, LOG_LEVEL lvl, LOG_MODE mode);
void log_set_level(LOG_LEVEL level);
void log_print(unsigned char lvl, const char *color, const char *file, const char *func, unsigned int line, const char *format, ...);
void log_destroy();

#define COL_DEF "\x1B[0m"   //white
#define COL_RED "\x1B[31m"  //red
#define COL_GRE "\x1B[32m"  //green
#define COL_BLU "\x1B[34m"  //blue
#define COL_YEL "\x1B[33m"  //yellow
#define COL_WHE "\x1B[37m"  //white
#define COL_CYN "\x1B[36m"
#define COL_MAG "\x1B[35m"

#define log_d(fmt,args...) \
    log_print(LOG_LEVEL_DEBUG,COL_WHE,__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define log_i(fmt,args...) \
    log_print(LOG_LEVEL_INFO,COL_GRE,__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define log_w(tag, fmt,args...) \
    log_print(LOG_LEVEL_WARN,COL_CYN,__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define log_e(fmt,args...) \
    log_print(LOG_LEVEL_ERR,COL_YEL,__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define log_f(fmt,args...) \
    log_print(LOG_LEVEL_FATAL,COL_RED,__FILE__,__FUNCTION__,__LINE__,fmt,##args)
#define log_m(fmt,args...) \
    log_print(LOG_LEVEL_MILESTONE,COL_BLU,__FILE__,__FUNCTION__,__LINE__,fmt,##args)

#if defined(__cplusplus) /* If this is a C++ compiler, use C linkage */
}
#endif

#endif /* __LOG_H__ */
