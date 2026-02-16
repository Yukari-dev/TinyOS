#ifndef APP_H
#define APP_h

typedef struct{
    char* name;
    void (*init)();
    void (*update)();
    void (*render)(int x, int y, int w, int h);
} AppInterfac;

#endif

