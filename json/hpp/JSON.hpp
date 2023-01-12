#pragma once

#include<map>
#include<string>
#include<list>
#include"Object.hpp"

using namespace std;

// JSON��
class JSON {

	// ���г�Ա
public:

	// ���캯������������
	JSON() = default;

	// ���ݼ�ֵ�Թ���
	JSON(map<string, Object>& copymap) :data(copymap) {}
	JSON(map<string, Object>&& copymap) :data(copymap) {}

	// ���ݶ���ֱ�ӹ��� 
	JSON(Object& obj) :data(obj.getObj()) {}
	JSON(Object&& obj) :data(obj.getObj()) {}

	~JSON() = default;



	//��̬����

	// ��̬���������ڽ�JSONת��Ϊ�ַ���
	static string stringify(const JSON& data);

	// ��̬���������ڽ�JSONת��Ϊ�ַ���
	static string stringify(JSON&& data);

	// ��̬���������ڽ��ַ���ת��ΪJSON
	static JSON parse(string jsonString);



	// ��Ա����
	
	// ���һ����ֵ��
	void push(const string& key, Object value);
	void push(string&& key, Object value);
	void push(const string& key, Object&& value);
	void push(string&& key, Object&& value);

	// ����һ����ֵ��
	void change(const string& key, Object value);
	void change(string&& key, Object value);
	void change(const string& key, Object&& value);
	void change(string&& key, Object&& value);

	// ɾ��ĳ����ֵ��
	void pop(const string& key);
	void pop(string&& key);

	// ��ȡJSON�е�˽�г�Ա��Ӱ���װ�ԣ�����ɾ����
	/*map<string, Object> getJSON(void);*/

	// ��ȡJSON�����м�
	Object getKey(void);

	// operator[]
	// �����ַ�����ȡ����
	Object& operator[](const string& idx);
	Object& operator[](string&& idx);



	// ˽�г�Ա
private:

	// �൱��һ������Ķ���
	map<string, Object> data;

};

// ��̬���������ڽ�JSONת��Ϊ�ַ���
string JSON::stringify(const JSON& json) {

	// ��ʼ�ַ��������Ա���
	string ans = "{";
	int len = json.data.size();
	int count = 0;

	// ��������map
	for (auto [key, val] : json.data) {
		ans += ("\"" + key + "\":" + val.toString());

		// �������һ��֮�ⶼҪ���϶���
		if (count != len - 1) ans += ",";
		count++;
	}

	// ����
	ans += "}";
	return ans;

}

// ��̬���������ڽ�JSONת��Ϊ�ַ���
string JSON::stringify(JSON&& json) {

	// ��ʼ�ַ��������Ա���
	string ans = "{";
	int len = json.data.size();
	int count = 0;

	// ��������map
	for (auto [key, val] : json.data) {
		ans += ("\"" + key + "\":" + val.toString());

		// �������һ��֮�ⶼҪ���϶���
		if (count != len - 1) ans += ",";
		count++;
	}

	// ����
	ans += "}";
	return ans;

}

// ���ַ���������JSON����
JSON JSON::parse(string jsonString) {
	return JSON(std::move(Object::parseObject(jsonString)));
}

// ��json���һ����ֵ��
inline void JSON::push(const string& key, Object value) { 

	// ���ü��Ѿ����ڹ������׳��쳣����Ҫ���������Ϣ
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// �����ǽ��ü���Ӧ��ֵ�޸�
	// ����ֱ���½���ֵ��
	this->data[key] = value;

}

inline void JSON::push(string&& key, Object value) {

	// ���ü��Ѿ����ڹ������׳��쳣����Ҫ���������Ϣ
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// �����ǽ��ü���Ӧ��ֵ�޸�
	// ����ֱ���½���ֵ��
	this->data[std::move(key)] = value;

}

inline void JSON::push(const string& key, Object&& value) {

	// ���ü��Ѿ����ڹ������׳��쳣����Ҫ���������Ϣ
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// �����ǽ��ü���Ӧ��ֵ�޸�
	// ����ֱ���½���ֵ��
	this->data[key] = std::move(value);

}

inline void JSON::push(string&& key, Object&& value) {

	// ���ü��Ѿ����ڹ������׳��쳣����Ҫ���������Ϣ
	if (this->data.count(key)) {
		cout << "[warn]:This key already existed!" << endl;
	}

	// �����ǽ��ü���Ӧ��ֵ�޸�
	// ����ֱ���½���ֵ��
	this->data[std::move(key)] = std::move(value);

}

// ������push��������ᷢ�����棬��;���ӷ���
inline void JSON::change(const string& key, Object value) {

	// ���ü������ڣ����׳��쳣����Ҫ���������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// ���ļ���Ӧ��ֵ
	this->data[key] = value;

}

inline void JSON::change(string&& key, Object value) {

	// ���ü������ڣ����׳��쳣����Ҫ���������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// ���ļ���Ӧ��ֵ
	this->data[std::move(key)] = value;

}

inline void JSON::change(const string& key, Object&& value) {

	// ���ü������ڣ����׳��쳣����Ҫ���������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// ���ļ���Ӧ��ֵ
	this->data[key] = std::move(value);

}

inline void JSON::change(string&& key, Object&& value) {

	// ���ü������ڣ����׳��쳣����Ҫ���������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key does not exist!" << endl;
	}

	// ���ļ���Ӧ��ֵ
	this->data[std::move(key)] = std::move(value);

}

// ɾ��ĳ����ֵ��
inline void JSON::pop(const string& key) {

	// ���ü������ڣ���Ҫ��ǰ�˳�����������ӡ������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key-value pair does not exist!" << endl;
		return;
	}

	// �����ڣ�ɾ���ü�ֵ��
	this->data.erase(key);
}

inline void JSON::pop(string&& key) {

	// ���ü������ڣ���Ҫ��ǰ�˳�����������ӡ������Ϣ
	if (!this->data.count(key)) {
		cout << "[warn]:This key-value pair does not exist!" << endl;
		return;
	}

	// �����ڣ�ɾ���ü�ֵ��
	this->data.erase(key);
}

inline Object JSON::getKey(void) {
	vector<Object> result;
	for (auto [key, val] : this->data) result.emplace_back(key);
	return result;
}

inline Object& JSON::operator[](const string& idx) {
	// �ڴ˴����� return ���
	if (!this->data.count(idx)) {
		cout << "[warn]:Invalid index!" << endl;
		Object obj;
		return obj;
	}
	return this->data[idx];
}

inline Object& JSON::operator[](string&& idx) {
	// �ڴ˴����� return ���
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