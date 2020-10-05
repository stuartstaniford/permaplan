
//#define LOG_BEZIER_FIT
#define LOG_DOUBLE_CLICK

#define LogStatement(...) fprintf(stderr, __DATE__ " " __TIME__ ": " __VA_ARGS__)

#ifdef LOG_BEZIER_FIT
#define LogBezierFit(...) LogStatement(__VA_ARGS__)
#else
#define LogBezierFit(...)
#endif

#ifdef LOG_DOUBLE_CLICK
#define LogDoubleClick(...) LogStatement(__VA_ARGS__)
#else
#define LogDoubleClick(...)
#endif
