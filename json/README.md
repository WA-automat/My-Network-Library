# json文件夹

主要包含四个文件: json.h、Object.hpp、JSON.hpp、JsonUtils.hpp、

其中json.h主要作用是引用后三个.hpp文件，在.cpp文件中调用，一般直接调用json.h文件

Object.hpp定义Object类，用于存储一个字符串（这里使用string）、数组（这里使用vector\<Object\>代替）、对象（这里使用map\<string, Object\>代替）

JSON.hpp主要用于以C++形式存储json对象，其成员包含一个map\<string, Object\>

JsonUtils用于将json文件导入C++程序中，将C++程序中的JSON对象写入json文件，以及JSON字符串的单行化与美化