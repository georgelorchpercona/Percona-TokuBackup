/* -*- mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*- */
// vim: ft=cpp:expandtab:ts=8:sw=4:softtabstop=4:
#ifndef BACKTRACE_H
#define BACKTRACE_H

#ident "Copyright (c) 2012-2013 Tokutek Inc.  All rights reserved."
#ident "$Id$"


class backtrace {
public:
    const char *file;
    const int line;
    const char *fun;
    const backtrace *prev;
    backtrace(void): file(0), line(-1), fun(0), prev(0) {}
    backtrace(const char *fi, int l, const char *fu, const backtrace *p) throw() : file(fi), line(l), fun(fu), prev(p) {}
};

#define BACKTRACE(bt) backtrace(__FILE__, __LINE__, __FUNCTION__, bt)
#endif
