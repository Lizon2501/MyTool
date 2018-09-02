#include <iostream>
#include <opencv2/opencv.hpp>  //ͷ�ļ�
#include <stdio.h>  
#include <time.h>  
#include <opencv/cv.h>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h> //�����ļ���غ���
////////////////////////////////////////////////
/*
����������������ͼƬ����·����txt�ļ���ȡ��������Ӧ��ͼƬ�����ָ��λ��

*/
//////////////////////////////////////////////////
using namespace cv::ml;
using namespace cv;  //����cv�����ռ�
using namespace std;

int main() {
	string txtpath = R"(F:\picture\txt\1.txt)";
	ifstream in(txtpath);
	vector<string> pictures;

	while (!in.eof()) {
		string picturepath;
		getline(in,picturepath);//ͨ��ϵͳ��������neg (#).jpg �и��ո�������Ҫ���ж���
		pictures.push_back(picturepath);
	}
	in.close();
	cout << pictures.size() << endl;
	cout << pictures[0] << endl;

	for (int i = 0; i < pictures.size()-1; i++) {//txt�ļ����һ�ж���һ���س�������
		Mat image = imread(pictures[i]);
		if (!image.empty()) {
			int pos = pictures[i].find_last_of('\\');
			string pictcure_out_path = "F:\\testpicture\\test_neg\\" + pictures[i].substr(pos + 1);//�ָ�ԭ�ַ����������ļ���
			imwrite(pictcure_out_path, image);
			cout << i << endl;
		}

	}

	getchar();
//	Mat tmp = imread("F:\\testpicture\\positive\\280.jpg",0);
	



	//waitKey(0);

}