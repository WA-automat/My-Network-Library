#pragma once

#include<string>
#include<stack>
#include<fstream>
#include<ios>
#include"JSON.hpp"

using namespace std;

typedef string JsonString;

class JsonUtils {

	// 公有成员
public:

	// 将普通字符串转化为连续的json字符串
	static JsonString uniline(const string& str);
	static JsonString uniline(string&& str);

	// 将json处理字符串美化成json文件中的字符串
	static string prettify(const JsonString& jsonString);
	static string prettify(JsonString&& jsonString);

	// 读取json文件
	static JSON readJsonFile(const string& jsonFile);
	static JSON readJsonFile(string&& jsonFile);

	// 写入json文件
	static void saveJsonFile(string jsonFile, const JSON& json);
	static void saveJsonFile(string jsonFile, JSON&& json);

};

// 将普通字符串转化为连续的json字符串
JsonString JsonUtils::uniline(const string& str) {

	// 字符串长度
	int len = str.size();

	// 判断当前是否遍历到双引号内部
	// 双引号内部的空格换行等内容不被删除
	bool flag = false;

	// 返回的字符串 
	JsonString jsonString;
	
	// 遍历字符串
	for (int i = 0; i < len; ++i) {

		// 变换
		if (str[i] == '\"') flag = !flag;

		// 当在双引号内部时
		if (flag) {
			jsonString += str[i];
		}

		// 当不在双引号内部时
		else {
			if (!isspace(str[i])) jsonString += str[i];
		}

	}

	return jsonString;
}

// 将普通字符串转化为连续的json字符串
inline JsonString JsonUtils::uniline(string&& str) {

	// 字符串长度
	int len = str.size();

	// 判断当前是否遍历到双引号内部
	// 双引号内部的空格换行等内容不被删除
	bool flag = false;

	// 返回的字符串 
	JsonString jsonString;

	// 遍历字符串
	for (int i = 0; i < len; ++i) {

		// 变换
		if (str[i] == '\"') flag = !flag;

		// 当在双引号内部时
		if (flag) {
			jsonString += str[i];
		}

		// 当不在双引号内部时
		else {
			if (!isspace(str[i])) jsonString += str[i];
		}

	}

	return jsonString;

}

// 美化，便于存放入json文件中
string JsonUtils::prettify(const JsonString& jsonString) {
	
	// 字符串长度
	int len = jsonString.size();

	// 栈区，用于存储左括号字符
	stack<char> box;

	// 返回的字符串
	string str;

	for (int i = 0; i <= len; ++i) {

		// 右括号要从栈中剔除元素
		if (jsonString[i] == ']') {

			// 出栈
			if (!box.empty() && box.top() == '[') {
				box.pop();
			}

			// 添加换行
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}
		else if (jsonString[i] == '}') {

			// 出栈
			if (!box.empty() && box.top() == '{') {
				box.pop();
			}

			// 添加换行
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

		// 当前字符为冒号时，前后加空格
		else if (jsonString[i] == ':') str += ' ';


		// 添加当前字符
		str += jsonString[i];


		// 当前字符为冒号时，前后加空格
		if (jsonString[i] == ':') str += ' ';

		// 左括号要增加换行
		// 并入栈
		else if (jsonString[i] == '[' || jsonString[i] == '{') {
			str += '\n';

			// 入栈
			box.push(jsonString[i]);

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

		// 逗号要增加换行
		else if (jsonString[i] == ',') {
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

	}

	return str;
}

// 美化，便于存放入json文件中
inline string JsonUtils::prettify(JsonString&& jsonString) {

	// 字符串长度
	int len = jsonString.size();

	// 栈区，用于存储左括号字符
	stack<char> box;

	// 返回的字符串
	string str;

	for (int i = 0; i <= len; ++i) {

		// 右括号要从栈中剔除元素
		if (jsonString[i] == ']') {

			// 出栈
			if (!box.empty() && box.top() == '[') {
				box.pop();
			}

			// 添加换行
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}
		else if (jsonString[i] == '}') {

			// 出栈
			if (!box.empty() && box.top() == '{') {
				box.pop();
			}

			// 添加换行
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

		// 当前字符为冒号时，前后加空格
		else if (jsonString[i] == ':') str += ' ';


		// 添加当前字符
		str += jsonString[i];


		// 当前字符为冒号时，前后加空格
		if (jsonString[i] == ':') str += ' ';

		// 左括号要增加换行
		// 并入栈
		else if (jsonString[i] == '[' || jsonString[i] == '{') {
			str += '\n';

			// 入栈
			box.push(jsonString[i]);

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

		// 逗号要增加换行
		else if (jsonString[i] == ',') {
			str += '\n';

			// 根据栈中元素个数增加tab数
			int count = box.size();
			while (count--) str += '\t';
		}

	}

	return str;

}

// 读取json文件
JSON JsonUtils::readJsonFile(const string& jsonFile) {

	// 使用文件流打开json文件
	fstream fs;
	fs.open(jsonFile, std::ios::in);

	// 打开失败的情况
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// 打开成功的情况
	string line, str;
	while (getline(fs, line)) {
		line += '\n';
		str += line;
	}

	// 单行化
	JsonString jsonString = JsonUtils::uniline(str);

	// 返回JSON类型
	return JSON::parse(jsonString);
}

// 读取json文件
JSON JsonUtils::readJsonFile(string&& jsonFile) {

	// 使用文件流打开json文件
	fstream fs;
	fs.open(jsonFile, std::ios::in);

	// 打开失败的情况
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// 打开成功的情况
	string line, str;
	while (getline(fs, line)) {
		line += '\n';
		str += line;
	}

	// 单行化
	JsonString jsonString = JsonUtils::uniline(str);

	// 返回JSON类型
	return JSON::parse(jsonString);
}

// 保存json
void JsonUtils::saveJsonFile(string jsonFile, const JSON& json) {

	// 将json转换成字符串形式
	string str = JsonUtils::prettify(JSON::stringify(json));

	// 使用文件流打开json文件
	fstream fs;
	fs.open(jsonFile, std::ios::out | std::ios::trunc);

	// 打开失败的情况
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// 写入json文件中
	fs << str;

#ifdef DEBUG
	cout << "[Debug]:Save json file success!" << endl;
#endif // DEBUG


}

// 保存json
void JsonUtils::saveJsonFile(string jsonFile, JSON&& json) {

	// 将json转换成字符串形式
	string str = JsonUtils::prettify(JSON::stringify(std::move(json)));

	// 使用文件流打开json文件
	fstream fs;
	fs.open(jsonFile, std::ios::out | std::ios::trunc);

	// 打开失败的情况
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// 写入json文件中
	fs << str;

#ifdef DEBUG
	cout << "[Debug]:Save json file success!" << endl;
#endif // DEBUG


}

