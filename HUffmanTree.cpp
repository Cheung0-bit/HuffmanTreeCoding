/*
 * @Description:Huffman code&decode
 * @Author: cheung0
 * @Date: 2021-11-14 20:25:38
 */
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <Windows.h>

using namespace std;

// 定义哈夫曼树节点
typedef struct {
	int weight;
	int parent;
	int l_child;
	int r_child;
	char data;
} HTNode, * HuffmanTree;
typedef char** HuffmanCode;

//字符种类数
static int N;

//自定义函数声明
void frequencyRecord(HuffmanTree& HT);

void select(HuffmanTree HT, int num, int& child1, int& child2);

void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC);

void zip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code);

void unzip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code);

void binaryCode();

//统计该文件中各种字符的频率
void frequencyRecord(HuffmanTree& HT) {
	HuffmanTree TEMP;
	TEMP = new HTNode[130];
	for (int i = 0; i < 130; ++i) {
		TEMP[i].weight = 0;
	}
	ifstream originFile("poem.txt");
	originFile.seekg(0);
	if (!originFile) {
		cout << "Can't find the file!" << endl;
	}
	else {
		char _data;
		cin.unsetf(ios::skipws);
		while (!originFile.eof()) {
			if (originFile.get(_data)) {
				TEMP[_data].data = _data;
				TEMP[_data].weight++;
			}
		}
		originFile.close();
	}
	for (int i = 0; i < 130; ++i) {
		if (TEMP[i].weight != 0) {
			N++;
		}
	}
	HT = new HTNode[2 * N];
	int k = 1;
	for (int i = 0; i < 130; ++i) {
		if (TEMP[i].weight != 0) {
			HT[k++] = TEMP[i];
		}
	}
}

//找出最小的两个叶子节点
void select(HuffmanTree HT, int num, int& child1, int& child2) {
	child1 = child2 = 0;
	int w1 = 0, w2 = 0;
	//Start finding...
	for (int i = 1; i <= num; ++i) {
		if (HT[i].parent == 0) {
			if (child1 == 0) {
				child1 = i;
				w1 = HT[i].weight;
				continue;
			}
			if (child2 == 0) {
				child2 = i;
				w2 = HT[i].weight;
				continue;
			}
			if (w1 > w2 && w1 > HT[i].weight) {
				child1 = i;
				w1 = HT[i].weight;
				continue;
			}
			if (w2 > w1 && w2 > HT[i].weight) {
				child2 = i;
				w2 = HT[i].weight;
				continue;
			}
		}
	}
	// 使得w1永远小于w2
	int temp;
	if (w1 > w2) {
		temp = child1;
		child1 = child2;
		child2 = temp;
	}
}

//对各字符进行 Huffman编码，显示每个字符的编码
void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC) {
	int m = 2 * N - 1;
	for (int i = 1; i <= N; ++i) {
		HT[i].parent = 0;
		HT[i].l_child = 0;
		HT[i].r_child = 0;
	}
	for (int i = N + 1; i <= m; ++i) {
		HT[i].weight = 0;
		HT[i].parent = 0;
		HT[i].l_child = 0;
		HT[i].r_child = 0;
		HT[i].data = '#';
	}
	int child1, child2;
	for (int i = N + 1; i <= m; i++) {
		select(HT, i - 1, child1, child2);
		HT[child1].parent = i;
		HT[child2].parent = i;
		HT[i].l_child = child1;
		HT[i].r_child = child2;
		HT[i].weight = HT[child1].weight + HT[child2].weight;
	}
	HC = new char* [N + 1];
	char* cd = new char[N];
	cd[N - 1] = '\0';
	int start, c, f;
	for (int i = 1; i <= N; i++) {
		start = N - 1;
		for (c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) {
			if (HT[f].l_child == c) cd[--start] = '0';
			else cd[--start] = '1';
		}
		HC[i] = new char[N - start];
		strcpy(HC[i], &cd[start]);
	}
	delete[] cd;
	for (int i = 1; i <= N; i++) {
		if (HT[i].data == '\n') {
			cout << "回车" << " " << HC[i] << endl;
		}
		else if (HT[i].data == ' ') {
			cout << "空格" << " " << HC[i] << endl;
		}
		else {
			cout << HT[i].data << " " << HC[i] << endl;;
		}
	}
}

//将该文件翻译成 Huffman编码文件
void zip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code) {
	ofstream codeFile("codefile.txt");
	ifstream originFile("poem.txt");
	if (!codeFile) {
		cout << "Can't find the file!" << endl;
	}
	else {
		char _data;
		cin.unsetf(ios::skipws);
		while (!originFile.eof()) {
			if (originFile.get(_data)) {
				for (int i = 1; i <= N; ++i) {
					if (HT[i].data == _data) {
						codeFile << HC[i];
						code.push_back(HC[i]);
					}
				}
			}
		}
	}
	codeFile.close();
}

//再将 Huffman编码文件翻译成源文件
void unzip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code) {
	ofstream decodeFile("decodefile.txt");
	if (!decodeFile) {
		cout << "Can't find the file!" << endl;
	}
	else {
		vector<string>::iterator v = code.begin();
		while (v != code.end()) {
			for (int i = 1; i <= N; ++i) {
				if (HC[i] == *v) {
					decodeFile << HT[i].data;
				}
			}
			v++;
		}
	}
	decodeFile.close();
}

//显示每个字符以一个字节进行二进制编码后的编码文件
void binaryCode() {
	ofstream binaryFile("binaryfile.txt");
	ifstream originFile("poem.txt");
	originFile.seekg(0);
	if (!originFile) {
		cout << "Can't find the file!" << endl;
	}
	else {
		char _data;
		cin.unsetf(ios::skipws);
		while (!originFile.eof()) {
			if (originFile.get(_data)) {
				bitset<8> data(_data);
				binaryFile << data;
			}
		}
		originFile.close();
	}
}

int main() {
	system("color 02");
	cout << "******************" << endl;
	cout << "*哈夫曼编码解码器*" << endl;
	cout << "******************" << endl;
	Sleep(1000);
	HuffmanTree HT;
	HuffmanCode HC;
	vector<string> code;
	cout << "\n\n\n需要进行编码的文件内容为：\n\n";
	system("more poem.txt");
	Sleep(2000);
	cout << "\n\n正在打开poem.txt进行二进制编码......" << endl;
	Sleep(2000);
	binaryCode();
	cout << "\n\n二进制编码内容为：" << endl;
	Sleep(2000);
	system("more binaryfile.txt");
	Sleep(2000);
	cout << "\n\n正在统计字符权重......" << endl;
	Sleep(2000);
	frequencyRecord(HT);
	HuffmanCoding(HT, HC);
	cout << "\n\n写入编码文件......" << endl;
	Sleep(2000);
	zip(HT, HC, code);
	cout << "\n\n编码结果为：" << endl;
	system("more codefile.txt");
	Sleep(2000);
	cout << "\n\n解码编码文件......" << endl;
	Sleep(2000);
	unzip(HT, HC, code);
	cout << "\n\n解码结果为：" << endl;
	Sleep(2000);
	system("more decodefile.txt");
	cout << "\n\n编码前占用字节空间：" << endl;
	Sleep(1000);
	ifstream file_before("binaryfile.txt", ios::binary | ios::ate);
	auto size_before = file_before.tellg();
	cout << size_before << endl;
	file_before.close();
	cout << "\n\n编码后占用字节空间：" << endl;
	Sleep(1000);
	ifstream file_after("codefile.txt", ios::binary | ios::ate);
	auto size_after = file_after.tellg();
	cout << size_after << endl;
	file_after.close();
	Sleep(1000);
	cout << "\n\n压缩率为：" << (static_cast<double>(size_before - size_after)) / size_before * 100.0 << "%" << endl;
	Sleep(100000);
	return 0;
}