#include <opencv2/opencv.hpp>
#include <iostream>
#include <io.h>
#include "tinystr.h"  
#include "tinyxml.h"
#include <direct.h>
#include <fstream>
#include <windows.h>
#include <algorithm>
using namespace std;
using namespace cv;
using namespace cv::ml;
/*

this process can get some positive picture and negative

*/
//box参数结构体
struct BoxSize
{
	int xMin;
	int yMin;
	int xMax;
	int yMax;
};



string path,filename;
long filenumber;
vector<string> picturename;
vector<double> negative_SIZE = { 1, 2, 0.5, 0.95, 1.05, 1.2, 0.8};

bool ReadParaXml(string m_strXmlPath, vector<BoxSize>& vecNode)
{
	BoxSize *pNode = new BoxSize;

	//读取xml文件中的参数值
	TiXmlDocument* Document = new TiXmlDocument();
	if (!Document->LoadFile(m_strXmlPath.c_str()))
	{
		std::cout << "无法加载xml文件！" << endl;
		cin.get();
		return false;
	}
	TiXmlElement* RootElement = Document->RootElement();		//根目录

	TiXmlElement* NextElement = RootElement->FirstChildElement();		//根目录下的第一个节点层
																		//for(NextElement;NextElement;NextElement = NextElement->NextSiblingElement())
	while (NextElement != NULL)		//判断有没有读完
	{
		if (NextElement->ValueTStr() == "path") {
			path = NextElement->GetText();
			picturename.push_back(path);
			//cout << path << endl;
		}
		else if (NextElement->ValueTStr() == "filename") {
			//filename = NextElement->GetText();
			//picturename.push_back(filename);
		}
		else if (NextElement->ValueTStr() == "object")		//读到object节点
		{

			//NextElement = NextElement->NextSiblingElement();

			TiXmlElement* BoxElement = NextElement->FirstChildElement();
			while (BoxElement->ValueTStr() != "bndbox")		//读到box节点
			{
				BoxElement = BoxElement->NextSiblingElement();
			}
			//索引到xmin节点
			TiXmlElement* xminElemeng = BoxElement->FirstChildElement();
			{
				//分别读取四个数值
				pNode->xMin = atof(xminElemeng->GetText());
				TiXmlElement* yminElemeng = xminElemeng->NextSiblingElement();
				pNode->yMin = atof(yminElemeng->GetText());
				TiXmlElement* xmaxElemeng = yminElemeng->NextSiblingElement();
				pNode->xMax = atof(xmaxElemeng->GetText());
				TiXmlElement* ymaxElemeng = xmaxElemeng->NextSiblingElement();
				pNode->yMax = atof(ymaxElemeng->GetText());

				//加入到向量中
				vecNode.push_back(*pNode);
			}
		}
		NextElement = NextElement->NextSiblingElement();
	}

	//释放内存
	delete pNode;
	delete Document;
	//cout << "完成xml的读取" << endl;
	return true;
}
void drawRectangle(Mat image, vector<BoxSize> data) {
	for (int i = 0; i < data.size(); i++) {
		rectangle(image, Rect(data[i].xMin, data[i].yMin, data[i].xMax - data[i].xMin, data[i].yMax - data[i].yMin),Scalar(244,167,185),1);//draw rectangle in the image
	}
}

void outPositiveSample(Mat image, vector<BoxSize> data) {

	

	for (int i = 0; i < data.size(); i++, filenumber++) {
		char str[100];
		string name;
		sprintf_s(str, "F:\\smoke_picture\\positive\\%d.jpg", filenumber);
		name = str;

		Mat imageROI = image(Rect(data[i].xMin, data[i].yMin, data[i].xMax - data[i].xMin, data[i].yMax - data[i].yMin));
		imwrite(name, imageROI);
		//cout << filenumber << endl;

	}

}

void getFiles(string path, vector<string>& files)
{
	//文件句柄  
	long long hFile = 0;
	//文件信息  
	struct _finddata_t fileinfo ;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之  
			//如果不是,加入列表  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void getXMLfile(string filepath, vector<string> &files) {
	
}
void demoPositivepicture_FromXML() {
	filenumber = 0;

	vector<string> files;
	getFiles("F:\\smokexml", files);//XML PATH


	vector<BoxSize> data;
	for (int i = 0; i < files.size(); i++)
	{
		data.clear();
		ReadParaXml(files[i], data);
		if (!data.empty()) {
			Mat image_demo = imread(path);
			if (image_demo.empty()) {
				cout << path << "erroe!!!!" << endl;
				continue;
			}
			outPositiveSample(image_demo, data);
		}
		cout << files[i] << endl;
	}
}

float DecideOverlap(const Rect &r1, const Rect &r2)

{
	int x1 = r1.x;
	int y1 = r1.y;
	int width1 = r1.width;
	int height1 = r1.height;
	int x2 = r2.x;
	int y2 = r2.y;
	int width2 = r2.width;
	int height2 = r2.height;
	int endx = max(x1 + width1, x2 + width2);
	int startx = min(x1, x2);
	int width = width1 + width2 - (endx - startx);
	int endy = max(y1 + height1, y2 + height2);
	int starty = min(y1, y2);
	int height = height1 + height2 - (endy - starty);
	float ratio = 0.0f;
	float Area, Area1, Area2;
	if (width <= 0 || height <= 0)
		return 0.0f;
	else
	{
		Area = width * height;
		Area1 = width1 * height1;
		Area2 = width2 * height2;
		ratio = Area / (Area1 + Area2 - Area);
	}
	return ratio;

}

void outNegativePicture(Mat image) {
	srand(time(NULL));
	long maxnumber;
	if (image.cols > image.rows) {
		maxnumber = image.rows;
	}
	else
	{
		maxnumber = image.cols;
	}

	if(image.cols >200 &&image.rows>200){
		long number = 100, minSize = 200, maxSize = maxnumber / 2, x, y, r1, r2;
		for (long i = 0; i < number; i++) {
			x = rand() % (image.cols - maxSize);
			y = rand() % (image.rows - maxSize);
			r1 = rand() % (maxSize - minSize + 1) + minSize;
			r2 = ((long)(r1 * negative_SIZE[rand() % negative_SIZE.size()]));
			char str[200];
			string name;
			sprintf_s(str, "F:\\picture\\negative0901\\%d.jpg", filenumber);
			name = str;

			filenumber++;
			if ((x + r1) < image.cols && (y + r2) < image.rows) {
				Mat imageROI = image(Rect(x, y, r1, r2));
				imwrite(name, imageROI);
			}
			else
			{
				filenumber--;
				i--;
			}



		}
	}

}
void outNegativePicture(Mat image, vector<BoxSize> data) {
	srand(time(0));
	long number = 100, minSize = 40, maxSize = 800, x, y, r1,r2;
	
	for (long i = 0; i < number; i++) {
		bool NotOverlap = true;
		x = rand() % (image.cols - maxSize * 2);
		y = rand() % (image.rows - maxSize * 2);
		r1 = rand() % (maxSize - minSize + 1) + minSize;
		r2 = ((long)(r1 * negative_SIZE[rand() % negative_SIZE.size()]));
	//	cout << "here" << endl;
		for (int count = 0; count < data.size(); count++) {
			if (DecideOverlap(Rect(x, y, r1, r2), Rect(data[count].xMin, data[count].yMin, data[count].xMax - data[count].xMin, data[count].yMax - data[count].yMin)) !=0.0){
				NotOverlap = false;
				i--;
				cout << "出现重叠   " << filenumber << endl;
				break;
			}
		}
		if (NotOverlap) {
			char str[200];
			string name;
			sprintf_s(str, "F:\\picture\\negative\\%d.jpg", filenumber);
			name = str;
			filenumber++;

			if ((x + r1) < image.cols && (y + r2) < image.rows) {
				Mat imageROI = image(Rect(x, y, r1, r2));
				imwrite(name, imageROI);
			}
			else
			{
				filenumber--;
				i--;
			}
		}
	}
}
int main(int argc, char** argv)
{
	filenumber = 0;
	vector<string> files, pictureList;
	getFiles("path", pictureList);//delete the path on 2nd September ,2018
	for (int i = 0; i < pictureList.size(); i++) {
		Mat image = imread(pictureList[i]);
		if (!image.empty()) {
			
			cout << pictureList.size() - i << pictureList[i] << endl;
			outNegativePicture(image);
			
		}
	}




	//demoPositivepicture_FromXML();



	/*
	Mat image = imread("D:\\test.jpg");
	imshow("test", image);
	waitKey(0);
	*/	
	/*
	vector<string> files,pictureList;
	getFiles("F:\\xmlData", files);//XML PATH
	getFiles("F:\\...",pictureList);

	vector<BoxSize> data;
	for (int i = 0; i < files.size(); i++)
	{
		data.clear();
		ReadParaXml(files[i], data);	
	}
	
	
	for (int i = 0; i <pictureList.size(); i++)
	{	
		Mat image = imread(pictureList[i]);
		bool isFind = false;
		int Position;

		for (int j = 0; j < picturename.size(); j++)//find any tag in picture
		{
			if (picturename[j].compare(pictureList[i]) == 0) {
				isFind = true;
				Position = j;
				break;
			}
		}


		if (!isFind) {
			outNegativePicture(image);
		}//not find	
		else{
			data.clear();
			ReadParaXml(files[Position], data);
			cout << data.size() << endl;
			outNegativePicture(image,data);
		}//find
		cout << pictureList[i] << " 剩余 " << pictureList.size() - i << endl;
	}
	*/


	//cout << files.size() << endl;
	//cout << picturename[0] << endl;
	
	
	getchar();
	return 0;
}