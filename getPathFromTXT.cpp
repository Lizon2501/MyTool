#include <iostream>
#include <opencv2/opencv.hpp>  //头文件
#include <stdio.h>  
#include <time.h>  
#include <opencv/cv.h>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h> //查找文件相关函数
////////////////////////////////////////////////
/*
这个程序用来将存放图片绝对路径的txt文件读取，并将对应的图片输出到指定位置

*/
//////////////////////////////////////////////////
using namespace cv::ml;
using namespace cv;  //包含cv命名空间
using namespace std;

int main() {
	string txtpath = R"(F:\picture\txt\1.txt)";
	ifstream in(txtpath);
	vector<string> pictures;

	while (!in.eof()) {
		string picturepath;
		getline(in,picturepath);//通过系统批量改名neg (#).jpg 有个空格，所以需要整行读入
		pictures.push_back(picturepath);
	}
	in.close();
	cout << pictures.size() << endl;
	cout << pictures[0] << endl;

	for (int i = 0; i < pictures.size()-1; i++) {//txt文件最后一行多了一个回车，跳过
		Mat image = imread(pictures[i]);
		if (!image.empty()) {
			int pos = pictures[i].find_last_of('\\');
			string pictcure_out_path = "F:\\testpicture\\test_neg\\" + pictures[i].substr(pos + 1);//分割原字符串，保留文件名
			imwrite(pictcure_out_path, image);
			cout << i << endl;
		}

	}

	getchar();
//	Mat tmp = imread("F:\\testpicture\\positive\\280.jpg",0);
	



	//waitKey(0);

}