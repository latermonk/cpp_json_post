
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <io.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/PrettyWriter.h"
#include "curl/curl.h"

using namespace rapidjson;
using namespace std;
using namespace cv;


// Main func begin
int main(int argc, const char** argv)
{
    //init
	char *url = "http://10.252.0.5:8081/AI_Recognition_platform//faceAI/faceCompare";
	//char *url = "http://10.1.141.76:8081/AI_Recognition_platform//faceAI/faceCompare";

    CURL* curl = curl_easy_init();


    //set header
	struct curl_slist* headers = NULL;
	//headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	headers = curl_slist_append(headers, "Content-Type:application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_URL, url);


    //compose json data content
	Document doc;
	doc.SetObject();    //key-value �൱��map
						//doc.Setvalue();        //������ �൱��vector
	Document::AllocatorType &allocator = doc.GetAllocator(); //��ȡ������

	doc.AddMember("image", "/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAA0ADQDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwDs9Eu4vsaBQRlRW6k6BcjcSa848O6g66XE8jdFGSfSr7eLZSxi0+ykuiDgNnArmW9j1m9EzulmxyUP41NFLHLE0Un3WBBUjse1cGviLWV2i7sFhBOM78j8OKk1ee8ngjAunt0kHzP0NPmV7E8t0aGiwf2dqeqaZGytBHKJoirZAVx0+ox+tb68DmuY8IRaMvmnTr17qdcpMTJkZJyevuK6giu2nsePVjabHBuKKaFNFWQcBYwI1uqxKpUrgemMUryXmnt5drbohPWVzgfgBUOiTn+z4HGQQoLfl0rpbe5tZ8+aiN0xkZryoy1Z7y2VzC02PUb24Z7vUTdJ3RU2Rr7DjOfWunWxTUtNNu4xKn3WAzxUOoXkFtY4iQZPbHSspfGdtbvDFawTO5PzZU4BqlvcfQ6HQtJbTopl3QlD3jjCH8fWr+Dwc1gW2rX8moxLPCYxIg3c55zzXQkgDH1rpoybPMxUbSDI70Uw9aK67o4bM87tgwtgcAAHoKQXDJgjOc5oQ+TM6HIUr3qCRuVweK8hn0Ck0y4b0xRefOxKoCWOM1Vt/E1rqE4FsUt4+Ms1u0rNzjhRjH41PZS5LZUEHrmpzZ3LyFrObyiOW46/WiD7j1ZqaC+o3UiT6km1QzGAsuwkdOV689a6cNxzWXo0UsUG2aQSS/eZweueMVok9q9Kila55GLleY4mim5orQ5bnn16Nr8dy/6NgVkSysH4PSiivKlue8xY7mRHBXGatLqd0zKA+3JAJXiiiiO5En7p6f4Z0yBtHlkkaSSTzDh2bkcLx6dzTety8fQL0NFFelS2PKq/ESbQO2aKKK1Mz//Z", allocator);
	doc.AddMember("group", "new", allocator);

	

	//3����doc�����ֵд���ַ���
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);  //PrettyWriter�Ǹ�ʽ����json�������Writer���ǻ��пո�ѹ�����json  
	//Writer<StringBuffer> writer(buffer);
	doc.Accept(writer);

	/*
	{
	"image": "/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAA0ADQDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwDs9Eu4vsaBQRlRW6k6BcjcSa848O6g66XE8jdFGSfSr7eLZSxi0+ykuiDgNnArmW9j1m9EzulmxyUP41NFLHLE0Un3WBBUjse1cGviLWV2i7sFhBOM78j8OKk1ee8ngjAunt0kHzP0NPmV7E8t0aGiwf2dqeqaZGytBHKJoirZAVx0+ox+tb68DmuY8IRaMvmnTr17qdcpMTJkZJyevuK6giu2nsePVjabHBuKKaFNFWQcBYwI1uqxKpUrgemMUryXmnt5drbohPWVzgfgBUOiTn+z4HGQQoLfl0rpbe5tZ8+aiN0xkZryoy1Z7y2VzC02PUb24Z7vUTdJ3RU2Rr7DjOfWunWxTUtNNu4xKn3WAzxUOoXkFtY4iQZPbHSspfGdtbvDFawTO5PzZU4BqlvcfQ6HQtJbTopl3QlD3jjCH8fWr+Dwc1gW2rX8moxLPCYxIg3c55zzXQkgDH1rpoybPMxUbSDI70Uw9aK67o4bM87tgwtgcAAHoKQXDJgjOc5oQ+TM6HIUr3qCRuVweK8hn0Ck0y4b0xRefOxKoCWOM1Vt/E1rqE4FsUt4+Ms1u0rNzjhRjH41PZS5LZUEHrmpzZ3LyFrObyiOW46/WiD7j1ZqaC+o3UiT6km1QzGAsuwkdOV689a6cNxzWXo0UsUG2aQSS/eZweueMVok9q9Kila55GLleY4mim5orQ5bnn16Nr8dy/6NgVkSysH4PSiivKlue8xY7mRHBXGatLqd0zKA+3JAJXiiiiO5En7p6f4Z0yBtHlkkaSSTzDh2bkcLx6dzTety8fQL0NFFelS2PKq/ESbQO2aKKK1Mz//Z",
	"group": "new"
	}

	*/

	cout << "==============send request===============" << endl;
	cout << endl << endl;

	cout << buffer.GetString() << endl;
	string s_out = string(buffer.GetString());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, s_out.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, s_out.length());

    //send request
	CURLcode  response = curl_easy_perform(curl);


	//curl_easy_cleanup(curl);

	cout << response << endl;



	cin.get();
	system("pause");
	return 0;
}

