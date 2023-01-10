// 调试状态
#define DEBUG

#include<iostream>
#include"../json/json.h"
#include"../utils/response.h"

using namespace std;
int main(void) {

	// 简单JSON操作

	// 读入input.json
	JSON json = JsonUtils::readJsonFile("input.json");
	cout << json << endl;

	json["data"]["name"] = "linzexun";
	cout << json << endl;

	// 保存到output.json中
	JsonUtils::saveJsonFile("output.json", json);


	// 使用Response

	// response
	Response response("登录失败", "400", "error");
	cout << JsonUtils::prettify(JSON::stringify(response)) << endl;
	cout << response["data"] << endl;
	response.setMsg("登录成功");
	response.setCode("200");
	map<string, Object> data;
	data["name"] = "linzexun";
	data["age"] = "19";
	data["state"] = "success";
	response.setData(data);

	cout << response.getKey() << endl;

	cout << JsonUtils::prettify(JSON::stringify(response)) << endl;

	system("pause");
	return 0;
}
