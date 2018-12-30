# epoll

```cpp
#include <sys/epoll.h>
```

## Relative definitions

```cpp
typedef union epoll_data {
    void        *ptr;
    int         fd;
    uint32_t    u32;
    uint64_t    u64;
} epoll_data_t;

struct epoll_event {
    uint32_t        events;     /* 一个bit mask，表明了要监听的事件 */
    epoll_data_t    data;
};
```

## API

### epoll_create()

```int epoll_create(int size)``` 创建一个新的epoll实例，返回实例的文件描述符；参数size从Linux 2.6.8之后将不在起作用。

### epoll_ctl()

```int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)``` 提供将要目标fd、目标事件集和目标epoll实例联系起来的功能。

- ```epfd``` epoll file descriptor
- ```op``` 指明要进行的操作
    - ```EPOLL_CTL_ADD``` 将 ```fd``` 和 ```epoll_event``` 注册到 ```epfd``` 指向的epoll实例
    - ```EPOLL_CTL_MOD``` 修改 ```epfd``` 指向的epoll实例上注册的 ```fd``` 关联的事件
    - ```EPOLL_CTL_DEL``` 删除 ```epfd``` 指向的epoll实例上注册的 ```fd``` ， ```event``` 可以为 ```NULL```

### epoll_wait()

```int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)```

函数将会阻塞调用者，直到下面条件中的任意一条发生：
1. 监听的事件个数达到了 ```maxevents``` 规定的值。
1. 阻塞的时间超过了 ```timeout``` 规定的毫秒数。
函数返回后， ```events``` 指向的区域将会存储已经发生的事件， 其中 ```epoll_event.events``` 为发生的事件的bit置一的值， ```epoll_data_t``` 为向epoll实例注册时传递的值。

## 参考

- [Linux Programmer's Mannual EPOLL](http://www.man7.org/linux/man-pages/man7/epoll.7.html)
- [Linux Programmer's Mannual EPOLL_CREATE](http://www.man7.org/linux/man-pages/man2/epoll_create.2.html)
- [Linux Programmer's Mannual EPOLL_CTL](http://www.man7.org/linux/man-pages/man2/epoll_ctl.2.html)
- [Linux Programmer's Mannual EPOLL_WAIT](http://www.man7.org/linux/man-pages/man2/epoll_wait.2.html)