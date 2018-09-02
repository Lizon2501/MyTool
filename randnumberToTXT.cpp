#include <iostream>
#include <io.h>
#include <direct.h>
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <string>
#include <time.h>
//////////////////////
/*

这个程序是生成一个顺序数组并将其打乱，然后拼合成一个文件路径，用作随机选取图片的作用
不建议再次运行，会导致原文件打乱


*/
////////////////////////
using namespace std;

int main() {
	int total_number = 196200;
	vector<int> pictureList;
	for (int i = 0; i < total_number; i++) {
		pictureList.push_back(i);
	}
	srand(time(NULL));

	int pos,tmp;

	for (int i = 0; i < total_number; i++) {//make randnumber
		pos = rand() % total_number;
		tmp = pictureList[i];
		pictureList[i] = pictureList[pos];
		pictureList[pos] = tmp;
	}

	pos = 0;
	for (int count = 0; count < 50; count++) {
		string txt_name = "F:\\picture\\txt1\\" + to_string(count) + ".txt";
		cout << txt_name << endl;
		ofstream SaveFile(txt_name);

		for (int i = 0; i < 3924; i++) {
			string picturename = "F:\\picture\\negative\\neg ("+to_string(pictureList[i+pos])+").jpg";
			if (pictureList[i + pos] == 0) {
				cout << "here:    ";
			}
			SaveFile << picturename << endl;
		}
		SaveFile.close();
		pos += 3924;
	}


	getchar();
	return 0;
}