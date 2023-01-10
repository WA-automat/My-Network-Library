#pragma once

#include"../../json/JSON.hpp"

class Response : public JSON {
public:
	Response(Object msg = "", Object code = "200", Object data = "") : JSON() {
		this->push("msg", msg);
		this->push("code", code);
		this->push("data", data);
	}

	
	// ������ʾ��Ϣ
	void setMsg(string msg) {
		(*this)["msg"] = msg;
	}

	// ����״̬��
	void setCode(string code) {
		(*this)["code"] = code;
	}

	// ��������
	void setData(Object data) {
		(*this)["data"] = data;
	}


	// ��ֹ��Response�������ɾ���޸� 
	
	// ���һ����ֵ��
	void push(string key, Object value) {
	
		if (key != "msg" && key != "code" && key != "data") cout << "[warn]:No modification!" << endl;
		else JSON::push(key, value);

	}

	// ����һ����ֵ��
	void change(string key, Object value) {

		if (key != "msg" && key != "code" && key != "data") cout << "[warn]:No modification!" << endl;
		else JSON::change(key, value);

	}

	// ɾ��ĳ����ֵ��
	void pop(string key) {

		// ����ɾ��
		cout << "[warn]:No modification!" << endl;

	}

};

