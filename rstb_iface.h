// rstb_iface.h - v0.0 UnknownRori <unknownrori@proton.me> - MIT
// 
// This is a single-header-file library that provides consistent vtable 
// and basic dynamic dispatch for C99 code, this code has a lot of pitfall
// by using `void*` So handle it with care
// 
//
// Example: 
#if 0
#include <stdio.h>
#include <string.h>
#include "rstb_iface.h"

#define Widget_Methods(X) \
    X(get_x, int, void*); \
    X(get_y, int, void*);
INTERFACE_DEFINE(Transform, Widget_Methods);

typedef struct Button {
    INTERFACE_BIND(Transform);
    int x;
    int y;
} Button;

int METHOD(Button, get_x)
{
    SELF(Button);
    return self->x;
}

int METHOD(Button, get_y)
{
    SELF(Button);
    return self->y;
}

VTABLE_DECL(Transform, Button, 
    .get_x = Button_get_x,
    .get_y = Button_get_y,
);

void METHOD(Button, init, int x, int y)
{
    SELF(Button);
    memset(self, 0, sizeof(*self));
    self->x = x;
    self->y = y;
    VTABLE_BIND(self, Button);
}

int main()
{
    Button btn;
    Button_init(&btn, 69, 67);

    int x = CALL(&btn, get_x);
    int y = CALL(&btn, get_y);
    printf("btn: {%d, %d}\n\n", x, y);
}
#endif
#pragma once

#ifndef RSTB_IFACE_H
#define RSTB_IFACE_H

#define _VTABLE_VAR_NAME _vt
#define METHOD_BINDS(NAME, RETURN, ...) \
    RETURN (*NAME)( __VA_ARGS__ )
#define INTERFACE_DEFINE(NAME, METHODS) \
    typedef struct NAME {               \
        METHODS(METHOD_BINDS)           \
    } NAME;
#define INTERFACE_BIND(NAME) const NAME* _VTABLE_VAR_NAME;

#define VTABLE_DECL(INTERFACE, TYPE, ...) \
    const INTERFACE TYPE##_vt = { __VA_ARGS__ }
#define VTABLE_BIND(VAR, TYPE) \
    ((VAR)->_VTABLE_VAR_NAME = &TYPE##_vt)

#define METHOD(TYPE, NAME, ...) \
    TYPE##_##NAME(void* _self, ##__VA_ARGS__)
#define STATIC_METHOD(TYPE, NAME, ...) \
    TYPE##_##NAME(__VA_ARGS__)
#define SELF(STRUCT) STRUCT* const self = _self
#define FIELD(X) self->X

#define CALL(VAR, NAME, ...) \
    ((VAR)->_VTABLE_VAR_NAME->NAME((VAR), ##__VA_ARGS__))

#endif // RSTB_IFACE_H
