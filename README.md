# cpp_json_post
a c/c++ demo for  send json post request use rapidJson &libURL



## RapidJSON
https://github.com/Tencent/rapidjson


```
RapidJSON 是只有头文件的 C++ 库。只需把 include/rapidjson 目录复制至系统或项目的 include 目录中。
```


download from github 

then put the whole include directory  to your project

run code to test 


```
	Document doc;
	doc.SetObject();		
```

https://blog.csdn.net/qq_35703954/article/details/82691030
https://www.cnblogs.com/buyishi/p/8598736.html



## libCURL
https://curl.haxx.se/libcurl/

**download**
https://curl.haxx.se/download.html


```
wget https://curl.haxx.se/download/curl-7.65.1.tar.gz
```
**compile for win**


**use in project**

### post http demo

```
#include <stdio.h>
#include <curl/curl.h>
 
int main(void)
{
  CURL *curl;
  CURLcode res;
 
  /* In windows, this will init the winsock stuff */ 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* get a curl handle */ 
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
    /* Now specify the POST data */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return 0;
}
```




## 图形和base64转换
http://tool.chinaz.com/tools/imgtobase/


## **read prcture ->Base64 -> Josn  -> psot **


```
void MatToJpg(const cv::Mat &mat, std::string &strImg)
{
	vector<uchar> buff;//buffer for coding 
	vector<int> param = vector<int>(2);
	param[0] = CV_IMWRITE_JPEG_QUALITY;
	param[1] = 95;//default(95) 0-100 
	imencode(".jpg", mat, buff, param);

	unsigned int image_size = buff.size();
	strImg.resize(image_size, '\0');
	uchar* image = (uchar*)strImg.data();

	for (unsigned int i = 0; i < image_size; i++)
		image[i] = (buff[i]);
	vector<uchar>().swap(buff);
	vector<int>().swap(param);
}


void trim(string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}

int main()
{
	string strIn;
	string strOut;

	Mat img = imread("./face.jpg");
	MatToJpg( img, strIn);
	unsigned char * uData = (unsigned char *)strIn.data();
	unsigned int nLen = strIn.length();
	strOut.resize(nLen * 2 + 1, '\0');
	DWORD nLenBase64 = strOut.length();
	base64::encode(uData, nLen, (unsigned char *)strOut.data(), &nLenBase64);
	
	trim(strOut);


	cout << "base64 code of the image = " << strOut << endl;

}








```




