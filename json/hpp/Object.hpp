#pragma once

#include<map>
#include<vector>
#include<string>
#include<stack>
#include"JSON.hpp"

using namespace std;

class Object {

	// ���г�Ա
public:

	// Ĭ�Ϲ��캯��
	Object() :_type(1), str_val("") {}

	// �ַ������캯��
	Object(string& val) :_type(1), str_val(val) {}
	Object(const string& val) :_type(1), str_val(val) {}
	Object(string&& val) :_type(1), str_val(val) {}

	// �ַ���ί�й��캯��
	Object(char str[]) :Object(string(str)) {}
	Object(const char str[]) :Object(string(str)) {}

	// ���鹹�캯��
	Object(const vector<Object>& list) :_type(2), array_val(list) {}
	Object(vector<Object>&& list) :_type(2), array_val(list) {}

	// JSONת��ΪObject��������캯��
	Object(const map<string, Object>& json) : _type(3), obj_val(json) {}
	Object(map<string, Object>&& json) : _type(3), obj_val(json) {}


	// Ĭ����������
	~Object() = default;



	// ��̬����

	// ���ڽ��ַ���ת���ɶ������͵ľ�̬����
	static Object parseObject(const string& objectString);
	static Object parseObject(string&& objectString);
	static Object parseObject(const string& objectString, int left, int right);
	static Object parseObject(string&& objectString, int left, int right);



	// ��Ա����

	// ��ȡ��������
	inline int getType(void);

	// ���ö�������
	inline void setType(int newType);

	// ��ȡ���������
	string getStr(void);
	vector<Object> getArray(void);
	map<string, Object> getObj(void);

	// ��ȡ�������͵ļ�������ֵΪ3��
	Object getKey(void);


	// ������ת��Ϊ�ַ���
	string toString(void);

	// ��ʽ����ת��Ϊstring
	operator string(void);

	// operator[]

	// ��������������ĵ�idx��ֵ
	Object& operator[](const int& idx);

	// ��������������ĵ�idx��ֵ
	Object& operator[](int&& idx);

	// �����ַ���������ֵ
	Object& operator[](const string& idx);

	// �����ַ���������ֵ
	Object& operator[](string&& idx);


	// ˽�г�Ա
private:
	// ����һ�����١�����union���������������ڴ��С��

	// �жϵ�ǰ���������
	// 0:��ʾ�ն���
	// 1:��ʾ�ַ�������
	// 2:��ʾ������������д�Ÿ�������
	// 3:��ʾ��������
	int _type;

	// �����Ա����
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

	// ������������ַ���
	if (objectString[left] == '\"' && objectString[right] == '\"') {

		// ֱ�ӷ����ַ���
		return Object(objectString.substr(left + 1, right - left - 1));

	}

	// ���������������
	else if (objectString[left] == '[' && objectString[right] == ']') {
		
		// ���ڴ洢���Ľ��
		vector<Object> arr;

		// ջ�������ڴ洢����Ų��ж��Ƿ�Ϸ�
		stack<char> box;

		// ��ǰ����ĵ�һ���ַ�������
		// ��ʼ�ĵ�һ���ַ�ΪobjectString[left + 1]
		int last = left + 1;

		// �����ַ���
		for (int i = left + 1; i <= right - 1; ++i) {

			// ��Ϊ˫����ʱ
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// ��Ϊ�������ʱ
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// ��Ϊ�Ұ�������ʱ
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

			// ��Ϊ�Ұ������ʱ
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

			// ��ջ��Ϊ��
			// ˵���Ѿ�ǰ��ƥ�䣬��Ҫ���Խ���Object����ȡ
			if (box.empty()) {

				// ��ȡ������
				Object obj = parseObject(objectString, last, i);

				// ����������
				arr.emplace_back(std::move(obj));

				// ��ƥ��ɹ�֮�󣬺���϶�����һ�����ţ���Ҫ����
				last = i + 2, ++i;

			}

		}

		// ջ��Ϊ��
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// ���ؽ��
		return Object(arr);

	}

	// ����������Ǽ�ֵ�ԵĶ���map
	else if (objectString[left] == '{' && objectString[right] == '}') {

		// ���ڴ洢���Ľ��
		map<string, Object> obj;

		// ջ�������ڴ洢����Ų��ж��Ƿ�Ϸ�
		stack<char> box;

		// ��ǰ����ĵ�һ���ַ�������
		// ��ʼ�ĵ�һ���ַ�ΪobjectString[left + 1]
		int keyLeft = left + 1, keyRight, valueLeft, valueRight;
		int last = left + 1;

		// �����жϵ�ǰ������key����value
		// ��ʼΪkey����true��ʾ
		// value��false��ʾ
		bool flag = true;

		// �����ַ���
		for (int i = left + 1; i <= right - 1; ++i) {

			// ��Ϊ˫����ʱ
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// ��Ϊ�������ʱ
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// ��Ϊ�Ұ�������ʱ
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

			// ��Ϊ�Ұ������ʱ
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

			// ��ջ��Ϊ��
			// ˵���Ѿ�ǰ��ƥ�䣬���Խ���key��value����ȡ
			if (box.empty()) {

				// ��Ϊkey
				if (flag) {
					
					// ���ı߽�
					keyRight = i, valueLeft = i + 2;

				}

				// ��Ϊvalue
				else {

					// ���ı߽�
					valueRight = i;

					// ��ȡ������ֵ
					Object key = parseObject(objectString, keyLeft, keyRight);
					Object value = parseObject(objectString, valueLeft, valueRight);

					// ��ӵ�map��
					obj[string(key)] = value;

					// �������µļ�������
					keyLeft = i + 2;

				}

				// ��ƥ��ɹ�֮�󣬺���϶�����һ�����Ż�ð�ţ���Ҫ����
				last = i + 2, ++i;

				// �ı�flag
				flag = !flag;

			}

		}

		// ջ��Ϊ��
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// ���ؽ��
		return Object(obj);

	}

	// ��������һ���������Ķ����ַ���
	// �׳��쳣
	else {
		cerr << "[error]:Invalid string!" << endl;
		throw runtime_error("Invalid string!");
	}

	return Object();
}

Object Object::parseObject(string&& objectString, int left, int right) {

	// ������������ַ���
	if (objectString[left] == '\"' && objectString[right] == '\"') {

		// ֱ�ӷ����ַ���
		return Object(objectString.substr(left + 1, right - left - 1));

	}

	// ���������������
	else if (objectString[left] == '[' && objectString[right] == ']') {

		// ���ڴ洢���Ľ��
		vector<Object> arr;

		// ջ�������ڴ洢����Ų��ж��Ƿ�Ϸ�
		stack<char> box;

		// ��ǰ����ĵ�һ���ַ�������
		// ��ʼ�ĵ�һ���ַ�ΪobjectString[left + 1]
		int last = left + 1;

		// �����ַ���
		for (int i = left + 1; i <= right - 1; ++i) {

			// ��Ϊ˫����ʱ
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// ��Ϊ�������ʱ
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// ��Ϊ�Ұ�������ʱ
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

			// ��Ϊ�Ұ������ʱ
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

			// ��ջ��Ϊ��
			// ˵���Ѿ�ǰ��ƥ�䣬��Ҫ���Խ���Object����ȡ
			if (box.empty()) {

				// ��ȡ������
				Object obj = parseObject(std::move(objectString), last, i);

				// ����������
				arr.emplace_back(std::move(obj));

				// ��ƥ��ɹ�֮�󣬺���϶�����һ�����ţ���Ҫ����
				last = i + 2, ++i;

			}

		}

		// ջ��Ϊ��
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// ���ؽ��
		return Object(arr);

	}

	// ����������Ǽ�ֵ�ԵĶ���map
	else if (objectString[left] == '{' && objectString[right] == '}') {

		// ���ڴ洢���Ľ��
		map<string, Object> obj;

		// ջ�������ڴ洢����Ų��ж��Ƿ�Ϸ�
		stack<char> box;

		// ��ǰ����ĵ�һ���ַ�������
		// ��ʼ�ĵ�һ���ַ�ΪobjectString[left + 1]
		int keyLeft = left + 1, keyRight, valueLeft, valueRight;
		int last = left + 1;

		// �����жϵ�ǰ������key����value
		// ��ʼΪkey����true��ʾ
		// value��false��ʾ
		bool flag = true;

		// �����ַ���
		for (int i = left + 1; i <= right - 1; ++i) {

			// ��Ϊ˫����ʱ
			if (objectString[i] == '\"') {
				if (box.empty()) box.push(objectString[i]);
				else if (box.top() == '\"') box.pop();
				else box.push(objectString[i]);
			}

			// ��Ϊ�������ʱ
			else if (objectString[i] == '[' || objectString[i] == '{') box.push(objectString[i]);

			// ��Ϊ�Ұ�������ʱ
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

			// ��Ϊ�Ұ������ʱ
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

			// ��ջ��Ϊ��
			// ˵���Ѿ�ǰ��ƥ�䣬���Խ���key��value����ȡ
			if (box.empty()) {

				// ��Ϊkey
				if (flag) {

					// ���ı߽�
					keyRight = i, valueLeft = i + 2;

				}

				// ��Ϊvalue
				else {

					// ���ı߽�
					valueRight = i;

					// ��ȡ������ֵ
					Object key = parseObject(std::move(objectString), keyLeft, keyRight);
					Object value = parseObject(std::move(objectString), valueLeft, valueRight);

					// ��ӵ�map��
					obj[string(key)] = value;

					// �������µļ�������
					keyLeft = i + 2;

				}

				// ��ƥ��ɹ�֮�󣬺���϶�����һ�����Ż�ð�ţ���Ҫ����
				last = i + 2, ++i;

				// �ı�flag
				flag = !flag;

			}

		}

		// ջ��Ϊ��
		if (!box.empty()) {
			cerr << "[error]:Invalid string!" << endl;
			throw runtime_error("Invalid string!");
		}

		// ���ؽ��
		return Object(obj);

	}

	// ��������һ���������Ķ����ַ���
	// �׳��쳣
	else {
		cerr << "[error]:Invalid string!" << endl;
		throw runtime_error("Invalid string!");
	}

	return Object();
}

inline int Object::getType(void) {
	// ��ȡ��ǰ����
	return this->_type;
}

inline void Object::setType(int newType) {
	// �������Ͳ��Ϸ�ʱ�����ı����ͣ��ų�������Ϣ
	if (newType < 0 || newType>3) {
		cout << "[warn]:The newType is undefined!";
		return;
	}

	// �����ͺϷ�ʱ���ı�����
	this->_type = newType;
}

// ��ȡ������ַ���
inline string Object::getStr(void) {
	if (!this->_type) return "";
	if (this->_type != 1) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->str_val;
}

// ��ȡ���������
inline vector<Object> Object::getArray(void) {
	if (this->_type != 2) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->array_val;
}

// ��ȡ����Ķ���map
inline map<string, Object> Object::getObj(void) {
	if (this->_type != 3) {
		cerr << "[error]:Incorrect type!" << endl;
		throw runtime_error("Incorrect type!");
	}
	return this->obj_val;
}

// ��ȡ���м�
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

// ������ת��Ϊ�ַ���
string Object::toString(void) {
	
	// ���ַ���
	if (this->_type == 0) return "\"\"";

	// ��ʾ����ֵ����Ϊ�ַ���
	else if (this->_type == 1) return "\"" + this->str_val + "\"";

	// ��ʾ����ֵ����Ϊ����
	else if (this->_type == 2) {

		string ans = "[";
		int len = this->array_val.size();

		// ��������
		for (int i = 0; i < len; ++i) {
			ans += this->array_val[i].toString();
			
			// �������һ��֮�⣬��Ҫ���϶���
			if (i != len - 1) ans += ",";
		}

		// ����
		ans += "]";
		return ans;
		
	}
	
	// ��ʾ����ֵ����Ϊ����
	else if (this->_type == 3) {
		
		string ans = "{";
		int len = this->obj_val.size();
		int count = 0;

		// ��������map
		for (auto [key, val] : this->obj_val) {
			ans += ("\"" + key + "\":" + val.toString());

			// �������һ��֮�ⶼҪ���϶���
			if (count != len - 1) ans += ",";
			count++;
		}

		// ����
		ans += "}";
		return ans;

	}
}

// ����ת��������
inline Object::operator string(void) {
	return this->getStr();
}

// ��������������ĵ�idx��ֵ
inline Object& Object::operator[](const int& idx) {
	// �ڴ˴����� return ���
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
	// �ڴ˴����� return ���
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
	// �ڴ˴����� return ���
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
	// �ڴ˴����� return ���
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