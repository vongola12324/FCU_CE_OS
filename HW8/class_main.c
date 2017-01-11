#include <stdio.h>
#include "class_ex1_class.h"

int main() {
    CRectangle *rect;
    //init class
    rect = init_CRectangle(5, 3);
    if (!rect) {
        printf("Cannot init CRectangle\n");
    }
    rect->set_parameters(rect, 2, 4);
    printf("area:%d\n", rect->area(rect));
    //exit class
    exit_CRectangle(rect);
    return 0;
}
