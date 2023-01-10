// ����״̬
#define DEBUG

#include<iostream>
#include"../json/json.h"
#include"../utils/response.h"

using namespace std;
int main(void) {

	// ��JSON����

	// ����input.json
	JSON json = JsonUtils::readJsonFile("input.json");
	cout << json << endl;

	json["data"]["name"] = "linzexun";
	cout << json << endl;

	// ���浽output.json��
	JsonUtils::saveJsonFile("output.json", json);


	// ʹ��Response

	// response
	Response response("��¼ʧ��", "400", "error");
	cout << JsonUtils::prettify(JSON::stringify(response)) << endl;
	cout << response["data"] << endl;
	response.setMsg("��¼�ɹ�");
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
