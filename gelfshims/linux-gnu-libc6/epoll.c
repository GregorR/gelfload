#include "shim.h"

int SHIM(epoll_create)(int size)
{
    return -1;
}

int SHIM(epoll_create1)(int flags)
{
    return -1;
}
