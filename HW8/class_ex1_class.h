#ifndef CLASS_EX1_CLASS
#define CLASS_EX1_CLASS

#include <stdio.h>
#include <stdlib.h>

typedef struct Rectangle {
    int width, height;
    int (*area)(struct Rectangle *);
    void (*set_parameters)(struct Rectangle *, int, int);
} 
CRectangle;
CRectangle *init_CRectangle(int width, int height);
int exit_CRectangle(CRectangle *pThis);

#endif
