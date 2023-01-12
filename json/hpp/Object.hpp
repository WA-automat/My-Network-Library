#pragma once

#include<map>
#include<vector>
#include<string>
#include<stack>
#include"JSON.hpp"

using namespace std;

class Object {

	// 公有成员
public:

	// 默认构造函数
	Object() :_type(1), str_val("") {}

	// 字符串构造函数
	Object(string& val) :_type(1), str_val(val) {}
	Object(const string& val) :_type(1), str_val(val) {}
	Object(string&& val) :_type(1), str_val(val) {}

	// 字符串委托构造函数
	Object(char str[]) :Object(string(str)) {}
	Object(const char str[]) :Object(string(str)) {}

	// 数组构造函数
	Object(const vector<Object>& list) :_type(2), array_val(list) {}
	Object(vector<Object>&& list) :_type(2), array_val(list) {}

	// JSON转换为Object用这个构造函数
	Object(const map<string, Object>& json) : _type(3), obj_val(json) {}
	Object(map<string, Object>&& json) : _type(3), obj_val(json) {}


	// 默认析构函数
	~Object() = default;



	// 静态函数

	// 用于将字符串转换成对象类型的静态函数
	static Object parseObject(const string& objectString);
	static Object parseObject(string&& objectString);
	static Object parseObject(const string& objectString, int left, int right);
	static Object parseObject(string&& objectString, int left, int right);



	// 成员函数

	// 获取对象类型
	inline int getType(void);

	// 设置对象类型
	inline void setType(int newType);

	// 获取对象的内容
	string getStr(void);
	vector<Object> getArray(void);
	map<string, Object> getObj(void);

	// 获取对象类型的键（类型值为3）
	Object getKey(void);


	// 将对象转化为字符串
	string toString(void);

	// 隐式类型转换为string
	operator string(void);

	// operator[]

	// 根据数字求数组的第idx个值
	Object& operator[](const int& idx);

	// 根据数字求数组的第idx个值
	Object& operator[](int&& idx);

	// 根据字符串求对象的值
	Object& operator[](const string& idx);

	// 根据字符串求对象的值
	Object& operator[](string&& idx);


	// 私有成员
private:
	// 构造一个“假”联合union（但这样会增加内存大小）

	// 判断当前对象的类型
	// 0:表示空对象
	// 1:表示字符串对象
	// 2:表示数组对象，数组中存放各种类型
	// 3:表示对象类型
	int _type;

	// 对象成员变量
	string str_val;
	vector<Object> array_val;
	map<string, Object> obj_val;

};

inline Object Object::parseObject(const string& objectString) {
	return parseObject(objectString, 0, objectString.size() - 1);
}

inline Object Object::parseObject(string&& objectString) {
	return parseObject(std::move(objectString), 0, objectString.size() - 1);
}

Object Object::parseObject(const string& objectString, int left, int right) {

	// 当这个对象是字符串
	if (objectString[left] == '\"' && objectString[right] == '\"') {

		// 直接返回字符串
		return Object(objectString.substr(left + 1, right - left - 1));

	}

	// 当这个对象是数组
	else if (objectString[left] == '[' && objectString[right] == ']') {
		
		// 用于存储最后的结果
		vector<Object> arr;

		// 栈区：用于存储左符号并判断是否合法
		stack<char> box;

		// 当前对象的第一个字符的索引
		// 初始的第一个字符为objectString[left + 1]
		int last = left + 1;

		// 遍历字符串
		for (int i = left + 1; i <= right - 1; ++i) {

			// 当为双引号时
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// 当为左半括号时
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// 当为右半中括号时
			else if (objectString[i] == ']') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '[') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当为右半大括号时
			else if (objectString[i] == '}') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '{') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当栈区为空
			// 说明已经前后匹配，需要可以进行Object的提取
			if (box.empty()) {

				// 提取出对象
				Object obj = parseObject(objectString, last, i);

				// 加入数组中
				arr.emplace_back(std::move(obj));

				// 当匹配成功之后，后面肯定会有一个逗号，需要跳过
				last = i + 2, ++i;

			}

		}

		// 栈不为空
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// 返回结果
		return Object(arr);

	}

	// 当这个对象是键值对的对象map
	else if (objectString[left] == '{' && objectString[right] == '}') {

		// 用于存储最后的结果
		map<string, Object> obj;

		// 栈区：用于存储左符号并判断是否合法
		stack<char> box;

		// 当前对象的第一个字符的索引
		// 初始的第一个字符为objectString[left + 1]
		int keyLeft = left + 1, keyRight, valueLeft, valueRight;
		int last = left + 1;

		// 用于判断当前对象是key还是value
		// 初始为key，用true表示
		// value用false表示
		bool flag = true;

		// 遍历字符串
		for (int i = left + 1; i <= right - 1; ++i) {

			// 当为双引号时
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// 当为左半括号时
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// 当为右半中括号时
			else if (objectString[i] == ']') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '[') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当为右半大括号时
			else if (objectString[i] == '}') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '{') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当栈区为空
			// 说明已经前后匹配，可以进行key或value的提取
			if (box.empty()) {

				// 若为key
				if (flag) {
					
					// 更改边界
					keyRight = i, valueLeft = i + 2;

				}

				// 若为value
				else {

					// 更改边界
					valueRight = i;

					// 提取出键和值
					Object key = parseObject(objectString, keyLeft, keyRight);
					Object value = parseObject(objectString, valueLeft, valueRight);

					// 添加到map中
					obj[string(key)] = value;

					// 更新最新的键的左半边
					keyLeft = i + 2;

				}

				// 当匹配成功之后，后面肯定会有一个逗号或冒号，需要跳过
				last = i + 2, ++i;

				// 改变flag
				flag = !flag;

			}

		}

		// 栈不为空
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// 返回结果
		return Object(obj);

	}

	// 否则，这是一个不正常的对象字符串
	// 抛出异常
	else {
		cerr << "[error]:Invalid string!" << endl;
		throw runtime_error("Invalid string!");
	}

	return Object();
}

Object Object::parseObject(string&& objectString, int left, int right) {

	// 当这个对象是字符串
	if (objectString[left] == '\"' && objectString[right] == '\"') {

		// 直接返回字符串
		return Object(objectString.substr(left + 1, right - left - 1));

	}

	// 当这个对象是数组
	else if (objectString[left] == '[' && objectString[right] == ']') {

		// 用于存储最后的结果
		vector<Object> arr;

		// 栈区：用于存储左符号并判断是否合法
		stack<char> box;

		// 当前对象的第一个字符的索引
		// 初始的第一个字符为objectString[left + 1]
		int last = left + 1;

		// 遍历字符串
		for (int i = left + 1; i <= right - 1; ++i) {

			// 当为双引号时
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// 当为左半括号时
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// 当为右半中括号时
			else if (objectString[i] == ']') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '[') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当为右半大括号时
			else if (objectString[i] == '}') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '{') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当栈区为空
			// 说明已经前后匹配，需要可以进行Object的提取
			if (box.empty()) {

				// 提取出对象
				Object obj = parseObject(std::move(objectString), last, i);

				// 加入数组中
				arr.emplace_back(std::move(obj));

				// 当匹配成功之后，后面肯定会有一个逗号，需要跳过
				last = i + 2, ++i;

			}

		}

		// 栈不为空
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// 返回结果
		return Object(arr);

	}

	// 当这个对象是键值对的对象map
	else if (objectString[left] == '{' && objectString[right] == '}') {

		// 用于存储最后的结果
		map<string, Object> obj;

		// 栈区：用于存储左符号并判断是否合法
		stack<char> box;

		// 当前对象的第一个字符的索引
		// 初始的第一个字符为objectString[left + 1]
		int keyLeft = left + 1, keyRight, valueLeft, valueRight;
		int last = left + 1;

		// 用于判断当前对象是key还是value
		// 初始为key，用true表示
		// value用false表示
		bool flag = true;

		// 遍历字符串
		for (int i = left + 1; i <= right - 1; ++i) {

			// 当为双引号时
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// 当为左半括号时
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// 当为右半中括号时
			else if (objectString[i] == ']') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '[') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当为右半大括号时
			else if (objectString[i] == '}') {
				if (box.empty()) {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
				else if (box.top() == '{') box.pop();
				else {
					cerr << "[error]:Invalid string!" << endl;
					throw runtime_error("Invalid string!");
				}
			}

			// 当栈区为空
			// 说明已经前后匹配，可以进行key或value的提取
			if (box.empty()) {

				// 若为key
				if (flag) {

					// 更改边界
					keyRight = i, valueLeft = i + 2;

				}

				// 若为value
				else {

					// 更改边界
					valueRight = i;

					// 提取出键和值
					Object key = parseObject(std::move(objectString), keyLeft, keyRight);
					Object value = parseObject(std::move(objectString), valueLeft, valueRight);

					// 添加到map中
					obj[string(key)] = value;

					// 更新最新的键的左半边
					keyLeft = i + 2;

				}

				// 当匹配成功之后，后面肯定会有一个逗号或冒号，需要跳过
				last = i + 2, ++i;

				// 改变flag
				flag = !flag;

			}

		}

		// 栈不为空
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// 返回结果
		return Object(obj);

	}

	// 否则，这是一个不正常的对象字符串
	// 抛出异常
	else {
		cerr << "[error]:Invalid string!" << endl;
		throw runtime_error("Invalid string!");
	}

	return Object();
}

inline int Object::getType(void) {
	// 获取当前类型
	return this->_type;
}

inline void Object::setType(int newType) {
	// 当新类型不合法时，不改变类型，放出警告信息
	if (newType < 0 || newType>3) {
		cout << "[warn]:The newType is undefined!";
		return;
	}

	// 新类型合法时，改变类型
	this->_type = newType;
}

// 获取对象的字符串
inline string Object::getStr(void) {
	if (!this->_type) return "";
	if (this->_type != 1) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->str_val;
}

// 获取对象的数组
inline vector<Object> Object::getArray(void) {
	if (this->_type != 2) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->array_val;
}

// 获取对象的对象map
inline map<string, Object> Object::getObj(void) {
	if (this->_type != 3) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->obj_val;
}

// 获取所有键
inline Object Object::getKey(void) {
	if (this->_type != 3) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	vector<Object> result;
	for (auto [key, val] : this->obj_val) result.emplace_back(key);

#ifdef DEBUG
	cout << "[Debug]:Get key success!" << endl;
#endif // DEBUG

	return std::move(result);
}

// 将对象转化为字符串
string Object::toString(void) {
	
	// 空字符串
	if (this->_type == 0) return "\"\"";

	// 表示对象值类型为字符串
	else if (this->_type == 1) return "\"" + this->str_val + "\"";

	// 表示对象值类型为数组
	else if (this->_type == 2) {

		string ans = "[";
		int len = this->array_val.size();

		// 遍历数组
		for (int i = 0; i < len; ++i) {
			ans += this->array_val[i].toString();
			
			// 除了最后一个之外，都要加上逗号
			if (i != len - 1) ans += ",";
		}

		// 结束
		ans += "]";
		return ans;
		
	}
	
	// 表示对象值类型为对象
	else if (this->_type == 3) {
		
		string ans = "{";
		int len = this->obj_val.size();
		int count = 0;

		// 遍历对象map
		for (auto [key, val] : this->obj_val) {
			ans += ("\"" + key + "\":" + val.toString());

			// 除了最后一个之外都要加上逗号
			if (count != len - 1) ans += ",";
			count++;
		}

		// 结束
		ans += "}";
		return ans;

	}
}

// 类型转换操作符
inline Object::operator string(void) {
	return this->getStr();
}

// 根据数字求数组的第idx个值
inline Object& Object::operator[](const int& idx) {
	// 在此处插入 return 语句
	if (this->_type != 2) {
		cout << "[warn]:Incorrect type!" << endl;
		Object obj;
		return obj;
	}
	if (idx >= this->array_val.size()) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->array_val[idx];
}

inline Object& Object::operator[](int&& idx) {
	// 在此处插入 return 语句
	if (this->_type != 2) {
		cout << "[warn]:Incorrect type!" << endl;
		Object obj;
		return obj;
	}
	if (idx >= this->array_val.size()) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->array_val[std::move(idx)];
}

inline Object& Object::operator[](const string& idx) {
	// 在此处插入 return 语句
	if (this->_type != 3) {
		cout << "[warn]:Incorrect type!" << endl;
		Object obj;
		return obj;
	}
	if (!this->obj_val.count(idx)) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->obj_val[idx];
}

inline Object& Object::operator[](string&& idx) {
	// 在此处插入 return 语句
	if (this->_type != 3) {
		cout << "[warn]:Incorrect type!" << endl;
		Object obj;
		return obj;
	}
	if (!this->obj_val.count(idx)) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->obj_val[std::move(idx)];
}

ostream& operator<<(ostream& os, Object obj) {
	os << obj.toString();
	return os;
}