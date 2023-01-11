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

	
	// 设置提示信息
	void setMsg(string msg) {
		(*this)["msg"] = msg;

#ifdef DEBUG
		cout << "[Debug]:Set Message!" << endl;
#endif // DEBUG

	}

	// 设置状态码
	void setCode(string code) {
		(*this)["code"] = code;

#ifdef DEBUG
		cout << "[Debug]:Set Code!" << endl;
#endif // DEBUG

	}

	// 设置数据
	void setData(Object data) {
		(*this)["data"] = data;

#ifdef DEBUG
		cout << "[Debug]:Set Data!" << endl;
#endif // DEBUG

	}


	// 禁止对Response进行添加删除修改 
	
	// 添加一个键值对
	void push(string key, Object value) {
	
		if (key != "msg" && key != "code" && key != "data") cout << "[warn]:No modification!" << endl;
		else JSON::push(key, value);

	}

	// 更改一个键值对
	void change(string key, Object value) {

		if (key != "msg" && key != "code" && key != "data") cout << "[warn]:No modification!" << endl;
		else JSON::change(key, value);

	}

	// 删除某个键值对
	void pop(string key) {

		// 不可删除
		cout << "[warn]:No modification!" << endl;

	}

};

