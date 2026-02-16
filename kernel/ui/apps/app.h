#ifndef APP_H
#define APP_H

typedef struct{
    char* name;
    void (*on_init)();
    void (*on_update)();
    void (*on_render)(int x, int y, int w, int h);
} AppInterface;

#endif

