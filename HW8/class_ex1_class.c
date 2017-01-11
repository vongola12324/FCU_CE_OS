#include "class_ex1_class.h"

static void SetParameter(CRectangle *rect, int a, int b) {
    rect->width = a;
    rect->height = b;
}

static int GetArea(CRectangle *rect) {
    return (rect->width) * (rect->height);
}

//global functions
CRectangle *init_CRectangle(int width, int height) {
    CRectangle *pThis;
    pThis = (CRectangle *)malloc(sizeof(CRectangle));
    if (pThis) {
        pThis->set_parameters = SetParameter;
        pThis->area = GetArea;
        pThis->width = width;
        pThis->height = height;
    }
    return pThis;
}

int exit_CRectangle(CRectangle *pThis) {
    if (pThis) {
        free(pThis);
        return 0;
    }
    return -1;
}

