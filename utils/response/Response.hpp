#pragma once

#include"../../json/hpp/JSON.hpp"

class Response : public JSON {
public:
	Response(Object msg = "", Object code = "200", Object data = "") : JSON() {
		this->push("msg", msg);
		this->push("code", code);
		this->push("data", data);

#ifdef DEBUG
		cout << "[Debug]:Create ResponseResult!" << endl;
#endif // DEBUG

	}

	
	// ������ʾ��Ϣ
	void setMsg(string msg) {
		(*this)["msg"] = msg;

#ifdef DEBUG
		cout << "[Debug]:Set Message!" << endl;
#endif // DEBUG

	}

	// ����״̬��
	void setCode(string code) {
		(*this)["code"] = code;

#ifdef DEBUG
		cout << "[Debug]:Set Code!" << endl;
#endif // DEBUG

	}

	// ��������
	void setData(Object data) {
		(*this)["data"] = data;

#ifdef DEBUG
		cout << "[Debug]:Set Data!" << endl;
#endif // DEBUG

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

