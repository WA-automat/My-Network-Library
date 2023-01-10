# utils文件夹

包含文件: response.h、Response.hpp

response.h用于引用Response.hpp，在.cpp文件中调用，一般直接调用response.h文件

Response.hpp包含Response类，继承自../json/JSON.hpp中的JSON类，响应体也是一个JSON