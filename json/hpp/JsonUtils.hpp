#pragma once

#include<string>
#include<stack>
#include<fstream>
#include<ios>
#include"JSON.hpp"

using namespace std;

typedef string JsonString;

class JsonUtils {

	// ���г�Ա
public:

	// ����ͨ�ַ���ת��Ϊ������json�ַ���
	static JsonString uniline(const string& str);
	static JsonString uniline(string&& str);

	// ��json�����ַ���������json�ļ��е��ַ���
	static string prettify(const JsonString& jsonString);
	static string prettify(JsonString&& jsonString);

	// ��ȡjson�ļ�
	static JSON readJsonFile(const string& jsonFile);
	static JSON readJsonFile(string&& jsonFile);

	// д��json�ļ�
	static void saveJsonFile(string jsonFile, const JSON& json);
	static void saveJsonFile(string jsonFile, JSON&& json);

};

// ����ͨ�ַ���ת��Ϊ������json�ַ���
JsonString JsonUtils::uniline(const string& str) {

	// �ַ�������
	int len = str.size();

	// �жϵ�ǰ�Ƿ������˫�����ڲ�
	// ˫�����ڲ��Ŀո��е����ݲ���ɾ��
	bool flag = false;

	// ���ص��ַ��� 
	JsonString jsonString;
	
	// �����ַ���
	for (int i = 0; i < len; ++i) {

		// �任
		if (str[i] == '\"') flag = !flag;

		// ����˫�����ڲ�ʱ
		if (flag) {
			jsonString += str[i];
		}

		// ������˫�����ڲ�ʱ
		else {
			if (!isspace(str[i])) jsonString += str[i];
		}

	}

	return jsonString;
}

// ����ͨ�ַ���ת��Ϊ������json�ַ���
inline JsonString JsonUtils::uniline(string&& str) {

	// �ַ�������
	int len = str.size();

	// �жϵ�ǰ�Ƿ������˫�����ڲ�
	// ˫�����ڲ��Ŀո��е����ݲ���ɾ��
	bool flag = false;

	// ���ص��ַ��� 
	JsonString jsonString;

	// �����ַ���
	for (int i = 0; i < len; ++i) {

		// �任
		if (str[i] == '\"') flag = !flag;

		// ����˫�����ڲ�ʱ
		if (flag) {
			jsonString += str[i];
		}

		// ������˫�����ڲ�ʱ
		else {
			if (!isspace(str[i])) jsonString += str[i];
		}

	}

	return jsonString;

}

// ���������ڴ����json�ļ���
string JsonUtils::prettify(const JsonString& jsonString) {
	
	// �ַ�������
	int len = jsonString.size();

	// ջ�������ڴ洢�������ַ�
	stack<char> box;

	// ���ص��ַ���
	string str;

	for (int i = 0; i <= len; ++i) {

		// ������Ҫ��ջ���޳�Ԫ��
		if (jsonString[i] == ']') {

			// ��ջ
			if (!box.empty() && box.top() == '[') {
				box.pop();
			}

			// ��ӻ���
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}
		else if (jsonString[i] == '}') {

			// ��ջ
			if (!box.empty() && box.top() == '{') {
				box.pop();
			}

			// ��ӻ���
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

		// ��ǰ�ַ�Ϊð��ʱ��ǰ��ӿո�
		else if (jsonString[i] == ':') str += ' ';


		// ��ӵ�ǰ�ַ�
		str += jsonString[i];


		// ��ǰ�ַ�Ϊð��ʱ��ǰ��ӿո�
		if (jsonString[i] == ':') str += ' ';

		// ������Ҫ���ӻ���
		// ����ջ
		else if (jsonString[i] == '[' || jsonString[i] == '{') {
			str += '\n';

			// ��ջ
			box.push(jsonString[i]);

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

		// ����Ҫ���ӻ���
		else if (jsonString[i] == ',') {
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

	}

	return str;
}

// ���������ڴ����json�ļ���
inline string JsonUtils::prettify(JsonString&& jsonString) {

	// �ַ�������
	int len = jsonString.size();

	// ջ�������ڴ洢�������ַ�
	stack<char> box;

	// ���ص��ַ���
	string str;

	for (int i = 0; i <= len; ++i) {

		// ������Ҫ��ջ���޳�Ԫ��
		if (jsonString[i] == ']') {

			// ��ջ
			if (!box.empty() && box.top() == '[') {
				box.pop();
			}

			// ��ӻ���
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}
		else if (jsonString[i] == '}') {

			// ��ջ
			if (!box.empty() && box.top() == '{') {
				box.pop();
			}

			// ��ӻ���
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

		// ��ǰ�ַ�Ϊð��ʱ��ǰ��ӿո�
		else if (jsonString[i] == ':') str += ' ';


		// ��ӵ�ǰ�ַ�
		str += jsonString[i];


		// ��ǰ�ַ�Ϊð��ʱ��ǰ��ӿո�
		if (jsonString[i] == ':') str += ' ';

		// ������Ҫ���ӻ���
		// ����ջ
		else if (jsonString[i] == '[' || jsonString[i] == '{') {
			str += '\n';

			// ��ջ
			box.push(jsonString[i]);

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

		// ����Ҫ���ӻ���
		else if (jsonString[i] == ',') {
			str += '\n';

			// ����ջ��Ԫ�ظ�������tab��
			int count = box.size();
			while (count--) str += '\t';
		}

	}

	return str;

}

// ��ȡjson�ļ�
JSON JsonUtils::readJsonFile(const string& jsonFile) {

	// ʹ���ļ�����json�ļ�
	fstream fs;
	fs.open(jsonFile, std::ios::in);

	// ��ʧ�ܵ����
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// �򿪳ɹ������
	string line, str;
	while (getline(fs, line)) {
		line += '\n';
		str += line;
	}

	// ���л�
	JsonString jsonString = JsonUtils::uniline(str);

	// ����JSON����
	return JSON::parse(jsonString);
}

// ��ȡjson�ļ�
JSON JsonUtils::readJsonFile(string&& jsonFile) {

	// ʹ���ļ�����json�ļ�
	fstream fs;
	fs.open(jsonFile, std::ios::in);

	// ��ʧ�ܵ����
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// �򿪳ɹ������
	string line, str;
	while (getline(fs, line)) {
		line += '\n';
		str += line;
	}

	// ���л�
	JsonString jsonString = JsonUtils::uniline(str);

	// ����JSON����
	return JSON::parse(jsonString);
}

// ����json
void JsonUtils::saveJsonFile(string jsonFile, const JSON& json) {

	// ��jsonת�����ַ�����ʽ
	string str = JsonUtils::prettify(JSON::stringify(json));

	// ʹ���ļ�����json�ļ�
	fstream fs;
	fs.open(jsonFile, std::ios::out | std::ios::trunc);

	// ��ʧ�ܵ����
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// д��json�ļ���
	fs << str;

#ifdef DEBUG
	cout << "[Debug]:Save json file success!" << endl;
#endif // DEBUG


}

// ����json
void JsonUtils::saveJsonFile(string jsonFile, JSON&& json) {

	// ��jsonת�����ַ�����ʽ
	string str = JsonUtils::prettify(JSON::stringify(std::move(json)));

	// ʹ���ļ�����json�ļ�
	fstream fs;
	fs.open(jsonFile, std::ios::out | std::ios::trunc);

	// ��ʧ�ܵ����
	if (!fs) {
		cout << "[error]:Cannot open the json!" << endl;
		throw runtime_error("Cannot open the json!");
	}

	// д��json�ļ���
	fs << str;

#ifdef DEBUG
	cout << "[Debug]:Save json file success!" << endl;
#endif // DEBUG


}

