
//#define LOG_BEZIER_FIT

#ifdef LOG_BEZIER_FIT
#define LogBezierFit(...) fprintf(stderr, __DATE__ " " __TIME__ ": " __VA_ARGS__)
#else
#define LogBezierFit(...)
#endif
