# utils文件夹

一些工具类，目前包括：

> 响应体类
>
> 阻塞队列



response.h用于引用response/Response.hpp，在.cpp文件中调用，一般直接调用response.h文件

container.h用于引用container文件夹中的内容



response文件夹：

Response.hpp包含Response类，继承自../json/JSON.hpp中的JSON类，响应体也是一个JSON



container文件夹：

BlockQueue.hpp包含BlockQueue类，实现阻塞队列
