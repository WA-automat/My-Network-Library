#pragma once

#include<map>
#include<string>
#include<list>
#include"Object.hpp"

using namespace std;

// JSON类
class JSON {

	// 公有成员
public:

	// 构造函数与析构函数
	JSON() = default;

	// 根据键值对构造
	JSON(map<string, Object>& copymap) :data(copymap) {}
	JSON(map<string, Object>&& copymap) :data(copymap) {}

	// 根据对象直接构造 
	JSON(Object& obj) :data(obj.getObj()) {}
	JSON(Object&& obj) :data(obj.getObj()) {}

	~JSON() = default;



	//静态函数

	// 静态函数，用于将JSON转换为字符串
	static string stringify(const JSON& data);

	// 静态函数，用于将JSON转换为字符串
	static string stringify(JSON&& data);

	// 静态函数，用于将字符串转换为JSON
	static JSON parse(string jsonString);



	// 成员函数
	
	// 添加一个键值对
	void push(const string& key, Object value);
	void push(string&& key, Object value);
	void push(const string& key, Object&& value);
	void push(string&& key, Object&& value);

	// 更改一个键值对
	void change(const string& key, Object value);
	void change(string&& key, Object value);
	void change(const string& key, Object&& value);
	void change(string&& key, Object&& value);

	// 删除某个键值对
	void pop(const string& key);
	void pop(string&& key);

	// 获取JSON中的私有成员（影响封装性，现已删除）
	/*map<string, Object> getJSON(void);*/

	// 获取JSON中所有键
	Object getKey(void);

	// operator[]
	// 根据字符串获取对象
	Object& operator[](const string& idx);
	Object& operator[](string&& idx);



	// 私有成员
private:

	// 相当于一个有序的对象
	map<string, Object> data;

};

// 静态函数，用于将JSON转换为字符串
string JSON::stringify(const JSON& json) {

	// 初始字符串与属性变量
	string ans = "{";
	int len = json.data.size();
	int count = 0;

	// 遍历对象map
	for (auto [key, val] : json.data) {
		ans += ("\"" + key + "\":" + val.toString());

		// 除了最后一个之外都要加上逗号
		if (count != len - 1) ans += ",";
		count++;
	}

	// 结束
	ans += "}";
	return ans;

}

// 静态函数，用于将JSON转换为字符串
string JSON::stringify(JSON&& json) {

	// 初始字符串与属性变量
	string ans = "{";
	int len = json.data.size();
	int count = 0;

	// 遍历对象map
	for (auto [key, val] : json.data) {
		ans += ("\"" + key + "\":" + val.toString());

		// 除了最后一个之外都要加上逗号
		if (count != len - 1) ans += ",";
		count++;
	}

	// 结束
	ans += "}";
	return ans;

}

// 将字符串解析成JSON类型
JSON JSON::parse(string jsonString) {
	return JSON(std::move(Object::parseObject(jsonString)));
}

// 向json添加一个键值对
inline void JSON::push(const string& key, Object value) { 

	// 当该键已经存在过，不抛出异常，但要输出警告信息
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// 但还是将该键对应的值修改
	// 或是直接新建键值对
	this->data[key] = value;

}

inline void JSON::push(string&& key, Object value) {

	// 当该键已经存在过，不抛出异常，但要输出警告信息
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// 但还是将该键对应的值修改
	// 或是直接新建键值对
	this->data[std::move(key)] = value;

}

inline void JSON::push(const string& key, Object&& value) {

	// 当该键已经存在过，不抛出异常，但要输出警告信息
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// 但还是将该键对应的值修改
	// 或是直接新建键值对
	this->data[key] = std::move(value);

}

inline void JSON::push(string&& key, Object&& value) {

	// 当该键已经存在过，不抛出异常，但要输出警告信息
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// 但还是将该键对应的值修改
	// 或是直接新建键值对
	this->data[std::move(key)] = std::move(value);

}

// 区别于push，这个不会发出警告，用途更加分明
inline void JSON::change(const string& key, Object value) {

	// 当该键不存在，不抛出异常，但要输出警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// 更改键对应的值
	this->data[key] = value;

}

inline void JSON::change(string&& key, Object value) {

	// 当该键不存在，不抛出异常，但要输出警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// 更改键对应的值
	this->data[std::move(key)] = value;

}

inline void JSON::change(const string& key, Object&& value) {

	// 当该键不存在，不抛出异常，但要输出警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// 更改键对应的值
	this->data[key] = std::move(value);

}

inline void JSON::change(string&& key, Object&& value) {

	// 当该键不存在，不抛出异常，但要输出警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// 更改键对应的值
	this->data[std::move(key)] = std::move(value);

}

// 删除某个键值对
inline void JSON::pop(const string& key) {

	// 当该键不存在，需要提前退出函数，并打印警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key-value pair does not exist!" << endl;
		return;
	}

	// 若存在，删除该键值对
	this->data.erase(key);
}

inline void JSON::pop(string&& key) {

	// 当该键不存在，需要提前退出函数，并打印警告信息
	if (!this->data.count(key)) {
		cout << "[warn]:This key-value pair does not exist!" << endl;
		return;
	}

	// 若存在，删除该键值对
	this->data.erase(key);
}

inline Object JSON::getKey(void) {
	vector<Object> result;
	for (auto [key, val] : this->data) result.emplace_back(key);
	return result;
}

inline Object& JSON::operator[](const string& idx) {
	// 在此处插入 return 语句
	if (!this->data.count(idx)) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->data[idx];
}

inline Object& JSON::operator[](string&& idx) {
	// 在此处插入 return 语句
	if (!this->data.count(idx)) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->data[idx];
}

ostream& operator<<(ostream& os, JSON json) {
	string jsonString = JSON::stringify(json);
	os << jsonString;
	return os;
}