#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <cstddef>

#include <sys/time.h>

template <class Base, class Derived>
bool isOfClass(const void * obj) {
    return dynamic_cast<Derived *>((Base *) obj) != NULL;
}

inline long getElapsedTimeInUS(timeval & t1, timeval & t2) {
    return (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
}

#endif



