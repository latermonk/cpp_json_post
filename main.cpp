
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <chrono>
#include <thread>
#include <io.h>
#include "FaceAPI.hpp"
#include "checkLicense.h"
//#include "HttpRequest.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/reader.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/PrettyWriter.h"
#include"curl/curl.h"
#include "Base64.h"
//#include "Base64.h"
//#include  <libcurl>
#include "ConvertImage.h"


using namespace rapidjson;
using namespace std;
using namespace cv;

struct Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    void tic() { start = std::chrono::high_resolution_clock::now(); }
    double toc()
    {
        std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - start;
        return diff.count() * 1000; //ms
    }
};

static void drawShape(const vector<int>& shape, Mat& img, Scalar color)
{
    int nPoint = shape.size() / 2;
    int radius = 2;
    for (int i = 0; i < nPoint; ++i)
        circle(img, Point((int)(shape[i * 2]), (int)(shape[i * 2 + 1])), radius, color, -1, 8, 0);

    if (nPoint == 72)
    {
        const int nComponents = 9;
        int comp1[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12 };
        int comp2[] = { 13,14,15,16,17,18,19,20,13,21 };
        int comp3[] = { 22,23,24,25,26,27,28,29,22 };
        int comp4[] = { 30,31,32,33,34,35,36,37,30,38 };
        int comp5[] = { 39,40,41,42,43,44,45,46,39 };
        int comp6[] = { 47,48,49,50,51,52,53,54,55,56,47 };
        int comp7[] = { 51,57,52 };
        int comp8[] = { 58,59,60,61,62,63,64,65,58 };
        int comp9[] = { 58,66,67,68,62,69,70,71,58 };

        int* idx[] = { comp1,comp2,comp3,comp4,comp5,comp6,comp7,comp8,comp9 };
        int nPoints[nComponents] = { 13,10,9,10,9,11,3,9,9 };

        for (int i = 0; i < nComponents; ++i)
        {
            for (int j = 0; j < nPoints[i] - 1; ++j)
                line(img, Point((int)(shape[idx[i][j] * 2]), (int)(shape[idx[i][j] * 2 + 1])), Point((int)(shape[idx[i][j + 1] * 2]), (int)(shape[idx[i][j + 1] * 2 + 1])), CV_RGB(0, 0, 255), 1, 8, 0);
        }
    }
}

static void drawShape(vector<Point>& shape, Mat& img, Scalar color)
{
    for (int i = 0; i < shape.size(); ++i)
    {
        circle(img, shape[i], 2, color, -1, 8, 0);
    }
}

static void drawFaceRect(const TrackedFaceInfo &faceInfo, Mat& img, Scalar color)
{
	//printf("===========11111111111111=============");
    int w = faceInfo.box.mWidth / 2;
    int x0 = faceInfo.box.mCenter_x - w;
    int y0 = faceInfo.box.mCenter_y - w;
    int x1 = faceInfo.box.mCenter_x + w;
    int y1 = faceInfo.box.mCenter_y + w;

    cv::line(img, Point(x0, y0), Point(x1, y0), color, 2, 8, 0);
    cv::line(img, Point(x1, y0), Point(x1, y1), color, 2, 8, 0);
    cv::line(img, Point(x1, y1), Point(x0, y1), color, 2, 8, 0);
    cv::line(img, Point(x0, y1), Point(x0, y0), color, 2, 8, 0);

    cv::putText(img, "id:" + std::to_string(faceInfo.face_id), Point(x0, y0 - 5), FONT_HERSHEY_SIMPLEX, 0.8, color, 1);
}

static void showErrorMessage(int errcode)
{
	//printf("===========222222222222222222=============");
    if (errcode == ErrCode::ERROR_LICENSE)
    {
        cerr << "License不正确！" << endl;
    }
    else if (errcode == ErrCode::NO_FACE_DETECTED)
    {
        cerr << "没有检测到人脸。请不要遮挡脸部正对镜头，提高脸部亮度！" << endl;
    }
    else if (errcode == ErrCode::MULTI_FACE_DETECTED)
    {
        cerr << "检测到多个人脸！" << endl;
    }
    else if (errcode == ErrCode::PITCH_OUT_OF_DOWN_MAX_RANGE)
    {
        cerr << "脸部的俯仰角太大，请调整一下姿态！" << endl;
    }
    else if (errcode == ErrCode::PITCH_OUT_OF_UP_MAX_RANGE)
    {
        cerr << "脸部的俯仰角太大，请调整一下姿态！" << endl;
    }
    else if (errcode == ErrCode::YAW_OUT_OF_LEFT_MAX_RANGE)
    {
        cerr << "脸部左右侧角太大，请调整一下姿态！" << endl;
    }
    else if (errcode == ErrCode::YAW_OUT_OF_RIGHT_MAX_RANGE)
    {
        cerr << "脸部左右侧角太大，请调整一下姿态！" << endl;
    }
    else if (errcode == ErrCode::POOR_ILLUMINATION)
    {
        cerr << "脸部光照太暗，请换到光照充足的地方再尝试！" << endl;
    }
    else if (errcode == ErrCode::IMG_BLURED)
    {
        cerr << "脸部模糊，请保持静止对准相机！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_LEFT_EYE)
    {
        cerr << "左眼被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_RIGHT_EYE)
    {
        cerr << "右眼被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_NOSE)
    {
        cerr << "鼻子被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_MOUTH)
    {
        cerr << "嘴巴被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_LEFT_CONTOUR)
    {
        cerr << "左侧脸颊被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_RIGHT_CONTOUR)
    {
        cerr << "右侧脸颊被遮挡！" << endl;
    }
    else if (errcode == ErrCode::OCCLUSION_CHIN_CONTOUR)
    {
        cerr << "下巴被遮挡！" << endl;
    }
    else if (errcode == ErrCode::FACE_NOT_COMPLETE)
    {
        cerr << "人脸不完整，超出图像边界！" << endl;
    }
    else
    {
        ; //cerr<<"正在识别..." << endl;
    }
}

static void write_jpg(const std::string &fileName, const std::vector<unsigned char> &jpg)
{
    ofstream fs(fileName, ios::binary);
    fs.write((const char *)&(jpg.front()), jpg.size());
    fs.close();
}

static void usage()
{
    cout << "usage: trackerDemo [path/to/assets] [path/to/video]" << endl
        << "If [path/to/video] is not set, USB camera will be used.";
}

static int authType = 0; //授权方式，0：鉴权码授权，其他：摄像头授权


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


//读入文件夹下指定格式的所有文件，本程序是读取文件夹下所有jpg图片
void getFiles(string path, string file_format, vector<string>& files)
{
	intptr_t   hFile = 0;//intptr_t和uintptr_t是什么类型:typedef long int/ typedef unsigned long int
	struct _finddata_t fileinfo;
	string p, file_formatName;
	if (0 != strcmp(file_format.c_str(), ""))
	{
		file_formatName = "\\*." + file_format;
	}
	else
	{
		file_formatName = "\\*";
	}
	if ((hFile = _findfirst(p.assign(path).append(file_formatName).c_str(), &fileinfo)) != -1)//assign方法可以理解为先将原字符串清空，然后赋予新的值作替换。
	{
		do
		{
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}


void delFiles(string path, string file_format, vector<string>& files)
{

}


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





// Main func begin
int main(int argc, const char** argv)
{
	
	/*
	string filePath = "E:\\work\\new\\python_face_capture\\src"; //正样本路径
	vector<string> files;
	string file_format = "jpg";
	getFiles(filePath, file_format, files);
	int number = files.size();//文件数量

	cout << "Number of picture = " << number << endl;

	for (int i = 0; i < number; i++)
	{
		Mat img = imread(files[i]);
	}
	*/


	
	CURL* curl = curl_easy_init();

#if 1
	//Public server 
	char *url = "http://10.252.0.5:8081/AI_Recognition_platform//faceAI/faceCompare";

#else
	//Private Server
	char *url = "http://10.1.40.4:8081/AI_Recognition_platform//faceAI/faceCompare";
#endif


	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8"); //without this 500 error
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_POST, 1);//设置为非0表示本次操作为POST
	curl_easy_setopt(curl, CURLOPT_URL, url);


	//cout << buffer.GetString() << endl;
	//string s_out = string(buffer.GetString());


	//当摄像头为非广电卓识摄像头时，需要通过授权码授权后才能正常调用算法
	//authType = 0;
	if (0 == authType)
	{
		if (!initLicense())
		{
			return -1;
		}
	}

	
    string assetFolder = "./asset";
    string inputVideo0 = "0";
	string inputVideo1 = "1";

	if (_access(assetFolder.c_str(), 0) != 0)
	{
		assetFolder = "./bin/asset";
	}

#if 0
	if (argc < 2)
	{
	    usage();
	    //return -1;
	}

	
	if (argc >= 2)
	{
	assetFolder = argv[1];
	}
	if (argc >= 4)
	{
	inputVideo0 = argv[2];
	inputVideo1 = argv[3];
	}
#endif
   

#if 0
	VideoCapture cam0;//, cam1;
	 open camera；
	if (!cam0.open(atoi(inputVideo0.c_str())))
	{
	printf("open camera %s failed.\n", inputVideo0.c_str());
	return -1;
	}
	if (!cam1.open(atoi(inputVideo1.c_str())))
	{
		printf("open camera %s failed.\n", inputVideo1.c_str());
		return -1;
	}
	cam0.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cam0.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	cam1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cam1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
#endif
	

#if 1
	MultiFaceTracker mTracker(assetFolder.c_str());
#else
	FaceTracker mTracker(assetFolder.c_str());
#endif
    



    // Set options
    //mTracker.set_isVerifyLive(1);
//	mTracker.set_isVerifyLive(0);
//	mTracker.set_live_thr(0.8f);
//    mTracker.set_isEnsureOneFace(false);
	//mTracker.set_isEnsureOneFace(true);
	//mTracker.isTrackBiggestFace(true);

    mTracker.set_max_collect_num(1);
    mTracker.set_crop_size(256,256);   // 设置截取的人脸尺寸
    mTracker.set_crop_enlarge_ratio(1.8f);
    mTracker.set_min_face_size(60);
    //mTracker.set_detect_interval_noface(500);
    //mTracker.set_detect_interval_hasface(1000);
    mTracker.set_facescore_thr(0.3f);
    mTracker.set_collect_interval(200);
    mTracker.set_strict_try_times(5);
    mTracker.set_isCheckQuality(true);
    //mTracker.set_isEnsureOneFace(false);
    mTracker.set_blur_thr(0.4f);
    mTracker.set_illum_thr(10.f);
    mTracker.set_occlu_thr(0.4f);
    mTracker.set_thread_num(2);

	//std::string winName0 = "cam0";  //, winName1 = "cam1";
    //namedWindow(winName0);
	//namedWindow(winName1);
    //moveWindow(winName0, 10, 10);
	//moveWindow(winName1, 300, 10);

    //cv::Mat frame0, frame1;
    bool finish_get_faceimg = false;
    Timer tmr;

    while (true)
    {
        tmr.tic();


        //cam0 >> frame0;
		/*
        if (frame0.empty())
        {
            cerr << "frame empty!" << endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
            continue;
        }*/

		//frame0 = cv::imread("1.jpg", 1);
		//frame0 = cv::imread("two_people.jpg", 1);
		//frame0 = cv::imread("lichenbingbing.jpegg", 1); 
		//frame0 = cv::imread("lichenbingbing2.jpg", 1);

		//read image from directory
		string filePath = "D:\\two"; //图片路径
		vector<string> files;
		vector<string>::iterator it;
		string file_format = "jpg";
		getFiles(filePath, file_format, files);
		int number = files.size();   //图片数量

		
		cout << "Number of picture = " << number << endl;

		
		//Handle
		//for (int i = 0; i < 3; i++)


		// handle images
		for (int i = 0; i < number; i++)
		{
			cout << "====picture number======" << i << endl;

			//vector<string> abc = files[0];
			//cout << "This vector = " << abc << endl;




			Mat frame0 = imread(files[i]);

			cout << files[0] << endl;

			ImgStruct img0{ frame0.data, frame0.rows, frame0.cols, ImgType::BGR };

#if 1
			int errcode = mTracker.track_and_collect(img0);
#else
			int errcode = mTracker.track(img0, 3); //hardCode max faceNum to 10
#endif

			cout << "errcode  = " << errcode << endl;
			showErrorMessage(errcode);

			int faceNum = mTracker.get_tracked_face_num();
			cout << "=======FACE NUMBER  =========" << faceNum << endl;


#if 0
			for (int iter = 0; iter < faceNum; ++iter)
			{
				const TrackedFaceInfo &face = mTracker.get_tracked_face(iter);  // 指定序号的人脸信息

				int faceCropNum = mTracker.get_face_crop_num(iter);

				if (faceCropNum <= 0)  // 此处正常返回1个采集的人脸
				{
					continue;
				}
				else
				{
					cout << faceCropNum << endl;
				}

				const FaceCrop &faceCrop = mTracker.get_face_crop(iter, faceCropNum - 1);


			}


#endif


#if 0
			for (int faceIdx = 0; faceIdx < faceNum; ++faceIdx)
			{
				int faceCropNum = mTracker.get_face_crop_num(faceIdx);   // always 0  ?

				cout << "faceCropNum = " << faceCropNum << endl;

			}

			
#endif



			 //Handle face
			{ 
				if (faceNum == 0)
				{
					finish_get_faceimg = false;
				}
				else
				{

					if (errcode == ErrCode::OK)
					{
						if (finish_get_faceimg == false)
						{
							cerr << "Face images are ready!" << endl;

							for (int faceIdx = 0; faceIdx < faceNum; ++faceIdx)
							{
								const TrackedFaceInfo &face = mTracker.get_tracked_face(faceIdx);  // 指定序号的人脸信息

								int faceCropNum = mTracker.get_face_crop_num(faceIdx);   // always 0  ?

								if (faceCropNum <= 0)  // 此处正常返回1个采集的人脸
								{
									continue;
								}
								else
								{
									cout << faceCropNum << endl;
								}
#if 0
								int faceCropNum = mTracker.get_face_crop_num(faceIdx);   // always 0  ?

								cout << "faceCropNum = " << faceCropNum << endl;
#endif




								//for (int n = 0; n < faceCropNum; ++n)
								{
									const FaceCrop &faceCrop = mTracker.get_face_crop(faceIdx, faceCropNum-1 );
									Mat canvas(faceCrop.rows, faceCrop.cols, CV_8UC4);
									int *pixeldata = (int*)(canvas.ptr(0));
									std::copy(faceCrop.img.begin(), faceCrop.img.end(), pixeldata);


			

									static int idx = 0;
									idx++;
									string fileStem = string("D://tmp//crop_") + std::to_string(idx);
									write_jpg("./face.jpg", faceCrop.jpeg);   //Write to  jpg file

			

									//Read image to Mat
									Mat img = imread("./face.jpg");

#if 1

									string strIn;
									string strOut;

									MatToJpg(img, strIn);
									unsigned char * uData = (unsigned char *)strIn.data();
									unsigned int nLen = strIn.length();
									if (nLen < 6) nLen = 10;
									strOut.resize(nLen * 2 + 1, '\0');
									DWORD nLenBase64 = strOut.length();
									base64::encode(uData, nLen, (unsigned char *)strOut.data(), &nLenBase64);


									trim(strOut);

									cout << "===========================" << endl << strOut << endl;
#endif


								  //Send post request

								  Document doc;
								  doc.SetObject();    //key-value 相当与map
								
								  Document::AllocatorType &allocator = doc.GetAllocator(); //获取分配器

								  //Add member
								  doc.AddMember("group", "new", allocator);
#if 1

								  doc.AddMember("image", StringRef(strOut.c_str(), strlen(strOut.c_str())), allocator);
								  //doc.AddMember("image", "/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAgGBgcGBQgHBwcJCQgKDBQNDAsLDBkSEw8UHRofHh0aHBwgJC4nICIsIxwcKDcpLDAxNDQ0Hyc5PTgyPC4zNDL/2wBDAQkJCQwLDBgNDRgyIRwhMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjIyMjL/wAARCAA0ADQDASIAAhEBAxEB/8QAHwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAAAgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKBkaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWGh4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXGx8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QAHwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREAAgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEIFEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOEhYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPExcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oADAMBAAIRAxEAPwDs9Eu4vsaBQRlRW6k6BcjcSa848O6g66XE8jdFGSfSr7eLZSxi0+ykuiDgNnArmW9j1m9EzulmxyUP41NFLHLE0Un3WBBUjse1cGviLWV2i7sFhBOM78j8OKk1ee8ngjAunt0kHzP0NPmV7E8t0aGiwf2dqeqaZGytBHKJoirZAVx0+ox+tb68DmuY8IRaMvmnTr17qdcpMTJkZJyevuK6giu2nsePVjabHBuKKaFNFWQcBYwI1uqxKpUrgemMUryXmnt5drbohPWVzgfgBUOiTn+z4HGQQoLfl0rpbe5tZ8+aiN0xkZryoy1Z7y2VzC02PUb24Z7vUTdJ3RU2Rr7DjOfWunWxTUtNNu4xKn3WAzxUOoXkFtY4iQZPbHSspfGdtbvDFawTO5PzZU4BqlvcfQ6HQtJbTopl3QlD3jjCH8fWr+Dwc1gW2rX8moxLPCYxIg3c55zzXQkgDH1rpoybPMxUbSDI70Uw9aK67o4bM87tgwtgcAAHoKQXDJgjOc5oQ+TM6HIUr3qCRuVweK8hn0Ck0y4b0xRefOxKoCWOM1Vt/E1rqE4FsUt4+Ms1u0rNzjhRjH41PZS5LZUEHrmpzZ3LyFrObyiOW46/WiD7j1ZqaC+o3UiT6km1QzGAsuwkdOV689a6cNxzWXo0UsUG2aQSS/eZweueMVok9q9Kila55GLleY4mim5orQ5bnn16Nr8dy/6NgVkSysH4PSiivKlue8xY7mRHBXGatLqd0zKA+3JAJXiiiiO5En7p6f4Z0yBtHlkkaSSTzDh2bkcLx6dzTety8fQL0NFFelS2PKq/ESbQO2aKKK1Mz//Z", allocator);
#endif

								  StringBuffer buffer;
								  PrettyWriter<StringBuffer> writer(buffer);  //PrettyWriter是格式化的json，如果是Writer则是换行空格压缩后的json  
								  doc.Accept(writer);

								  string s_out3 = string(buffer.GetString());

								  cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
								  cout << "s_out3" << endl << s_out3 << endl;
								  cout << "###################################" << endl;


								  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, s_out3.c_str());
								  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, s_out3.length());


								  CURLcode  response = curl_easy_perform(curl);

								  //curl_easy_cleanup(curl);


								  cout << response << endl;


								}

							//end of for


							}

							
						}
					}
				}



				
			}
		
			

			//Delete handled image here
#if 1
			string  fileToDelete = files[i];
			if (remove(fileToDelete.c_str()) == 0)
			{
				cout << "删除成功" << endl;
			}
			else
			{
				cout << "删除失败" << endl;
			}
#endif
		}


		//OUT OF the BIG FOR 
		//Delete for
		cout << "Begin delete handled image ........." << endl << endl;
		//files.clear();


		//system("pause");


       // if (waitKey(1) == 27)
         //   break;
    }



	//END OF THE WHILE-TRUE



	//clean the resource
	curl_easy_cleanup(curl);


    return 0;
}

