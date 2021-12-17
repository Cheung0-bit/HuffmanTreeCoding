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

// ������������ڵ�
typedef struct {
	int weight;
	int parent;
	int l_child;
	int r_child;
	char data;
} HTNode, * HuffmanTree;
typedef char** HuffmanCode;

//�ַ�������
static int N;

//�Զ��庯������
void frequencyRecord(HuffmanTree& HT);

void select(HuffmanTree HT, int num, int& child1, int& child2);

void HuffmanCoding(HuffmanTree& HT, HuffmanCode& HC);

void zip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code);

void unzip(HuffmanTree& HT, HuffmanCode& HC, vector<string>& code);

void binaryCode();

//ͳ�Ƹ��ļ��и����ַ���Ƶ��
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

//�ҳ���С������Ҷ�ӽڵ�
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
	// ʹ��w1��ԶС��w2
	int temp;
	if (w1 > w2) {
		temp = child1;
		child1 = child2;
		child2 = temp;
	}
}

//�Ը��ַ����� Huffman���룬��ʾÿ���ַ��ı���
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
			cout << "�س�" << " " << HC[i] << endl;
		}
		else if (HT[i].data == ' ') {
			cout << "�ո�" << " " << HC[i] << endl;
		}
		else {
			cout << HT[i].data << " " << HC[i] << endl;;
		}
	}
}

//�����ļ������ Huffman�����ļ�
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

//�ٽ� Huffman�����ļ������Դ�ļ�
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

//��ʾÿ���ַ���һ���ֽڽ��ж����Ʊ����ı����ļ�
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
	cout << "*���������������*" << endl;
	cout << "******************" << endl;
	Sleep(1000);
	HuffmanTree HT;
	HuffmanCode HC;
	vector<string> code;
	cout << "\n\n\n��Ҫ���б�����ļ�����Ϊ��\n\n";
	system("more poem.txt");
	Sleep(2000);
	cout << "\n\n���ڴ�poem.txt���ж����Ʊ���......" << endl;
	Sleep(2000);
	binaryCode();
	cout << "\n\n�����Ʊ�������Ϊ��" << endl;
	Sleep(2000);
	system("more binaryfile.txt");
	Sleep(2000);
	cout << "\n\n����ͳ���ַ�Ȩ��......" << endl;
	Sleep(2000);
	frequencyRecord(HT);
	HuffmanCoding(HT, HC);
	cout << "\n\nд������ļ�......" << endl;
	Sleep(2000);
	zip(HT, HC, code);
	cout << "\n\n������Ϊ��" << endl;
	system("more codefile.txt");
	Sleep(2000);
	cout << "\n\n��������ļ�......" << endl;
	Sleep(2000);
	unzip(HT, HC, code);
	cout << "\n\n������Ϊ��" << endl;
	Sleep(2000);
	system("more decodefile.txt");
	cout << "\n\n����ǰռ���ֽڿռ䣺" << endl;
	Sleep(1000);
	ifstream file_before("binaryfile.txt", ios::binary | ios::ate);
	auto size_before = file_before.tellg();
	cout << size_before << endl;
	file_before.close();
	cout << "\n\n�����ռ���ֽڿռ䣺" << endl;
	Sleep(1000);
	ifstream file_after("codefile.txt", ios::binary | ios::ate);
	auto size_after = file_after.tellg();
	cout << size_after << endl;
	file_after.close();
	Sleep(1000);
	cout << "\n\nѹ����Ϊ��" << (static_cast<double>(size_before - size_after)) / size_before * 100.0 << "%" << endl;
	Sleep(100000);
	return 0;
}