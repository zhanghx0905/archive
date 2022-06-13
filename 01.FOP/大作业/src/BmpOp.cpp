3.#include<iostream>
#include <Windows.h>
#include<string>
using namespace std;

extern int m, n;
extern unsigned char *pBmpBuf;    //����ͼ�����ݵ�ָ��
int bmpWidth;              //ͼ��Ŀ�
int bmpHeight;             //ͼ��ĸ�
int biBitCount;            //ͼ�����ͣ�ÿ����λ��
int linebyte;              //ͼ��ÿ�е��ֽ���
RGBQUAD *pColorTable;      //��ɫ��ָ��

//����ͼƬ 
bool Readbmp(string str) {
	const char* bmpname = str.data();
	FILE* fin;
	fin = fopen(bmpname, "rb");
	if (!fin) {
		cout << "ͼƬ��ȡʧ�ܣ�\n";
		return 0;
	}
	//����bmp�ļ�ͷ
	BITMAPFILEHEADER filehead;
	fread(&filehead, 1, sizeof(BITMAPFILEHEADER), fin);
	int bmpOffset = filehead.bfOffBits;
	//��ȡbmp��Ϣͷ
	BITMAPINFOHEADER infohead;
	fread(&infohead, sizeof(BITMAPINFOHEADER), 1, fin);

	biBitCount = infohead.biBitCount;
	//����Ƕ�ʮ��λ����ʮ��λͼ
	if (biBitCount == 24||biBitCount==32) {
		int inilinebyte = (infohead.biWidth*biBitCount / 8 + 3) / 4 * 4;
		//��ͼ����Ϣ����ȡ������
		linebyte = (infohead.biWidth*biBitCount / 8) / 3 / 4 / m * m * 4 * 3;
		bmpWidth = linebyte / (biBitCount / 8);
		bmpHeight = infohead.biHeight / n * n;
		int cutheight = bmpHeight / n;
		//�����ÿ���ֽڵĲ�ֵ
		int diffwidthbyte = inilinebyte - linebyte;
		fseek(fin, bmpOffset, 0);
		//����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
		pBmpBuf = new unsigned char[linebyte*bmpHeight];
		//�ֶζ������ݣ�ע��bmp�ļ��е������Ǵ����µ����ϼ�¼��
		for (int i = n - 1; i >= 0; i--)
			for (int j = 0; j < cutheight; j++) {
				fread(pBmpBuf + (i*cutheight + j)*linebyte, 1, linebyte, fin);
				//��������Ĳ��� 
				fseek(fin, diffwidthbyte, SEEK_CUR);
			}
		fclose(fin);
		cout << "ͼƬ��ȡ�ɹ���\n";
		return 1;
	}
	/*
	//�����8λͼ������Ҫ������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
	if (biBitCount == 8) {
		bmpWidth = infohead.biWidth;
		bmpHeight = infohead.biHeight;
		biBitCount = infohead.biBitCount;
		int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;//�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
		int cutheight = bmpHeight / n;
		//�Ҷ�ͼ������ɫ������ɫ�����Ϊ256
		if (biBitCount == 8) {
			//������ɫ������Ҫ�Ŀռ䣬����ɫ����ڴ�
			pColorTable = new RGBQUAD[256];
			fread(pColorTable, sizeof(RGBQUAD), 256, fin);
		}
		fseek(fin, bmpOffset, 0);
		pColorTable = new RGBQUAD[256];
		fread(&pColorTable, sizeof(RGBQUAD), 1, fin);
		for (int i = n - 1; i >= 0; i--)
			for (int j = 0; j < cutheight; j++) {
				fread(pBmpBuf + (i*cutheight + j)*linebyte, 1, linebyte, fin);
			}
		fclose(fin);
		cout << "ͼƬ��ȡ�ɹ���\n";
		return 1;
	}*/
	else cout << "�����򲻽������ָ�ʽ��ͼ��";
	return 0;
}
//�и����ͼƬ
bool Savebmp(int puzzle[]) {
	if (!pBmpBuf) {
		cout << "δ��⵽ͼƬ���룡\n";
		return 0;
	}
	//������������в���������ֱ�Ӹ���ԭͼ��
	unsigned char *pBmpBuffer = new unsigned char[linebyte*bmpHeight];
	memcpy(pBmpBuffer, pBmpBuf, linebyte*bmpHeight);
	string str;
	cout << "�����뽫Ҫ�����bmpͼƬ������·��������ͬһĿ¼����ֱ�������ļ���(������׺.bmp)������P�˳���ͼ����:\n";
	cin >> str;
	if (str == "P") return 0;
	const char* bmpname = str.data();
	int colorTablesize = 0;
	/*
	if (biBitCount == 8)
		colorTablesize = 1024;*/
	FILE *fout;
	fout = fopen(bmpname, "wb");

	if (!fout) {
		cout << "ͼƬ����ʧ�ܣ�\n";
		return 0;
	}

	BITMAPFILEHEADER filehead;
	filehead.bfType = 0x4D42;//bmp����
	filehead.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTablesize + linebyte * bmpHeight;
	filehead.bfReserved1 = 0;
	filehead.bfReserved2 = 0;
	filehead.bfOffBits = 54 + colorTablesize;
	fwrite(&filehead, sizeof(BITMAPFILEHEADER), 1, fout);

	BITMAPINFOHEADER infohead;
	infohead.biBitCount = biBitCount;
	infohead.biClrImportant = 0;
	infohead.biClrUsed = 0;
	infohead.biCompression = 0;
	infohead.biHeight = bmpHeight;
	infohead.biPlanes = 1;
	infohead.biSize = 40;
	infohead.biSizeImage = linebyte * bmpHeight;
	infohead.biWidth = bmpWidth;
	infohead.biXPelsPerMeter = 0;
	infohead.biYPelsPerMeter = 0;
	fwrite(&infohead, sizeof(BITMAPINFOHEADER), 1, fout);
	//�����8λͼ����д���ɫ��
	/*
	if (biBitCount == 8)
		fwrite(pColorTable, sizeof(RGBQUAD), 1, fout);*/

	int cutheight = bmpHeight / n;
	int blocklinebyte = linebyte / m;

	//�и�ͼƬ�����ֿհ׿�
	for (int i = 0; i < bmpHeight; i++)
		for (int j = 0; j < m; j++) {
			//����֮��Ϳ��
			if (i%cutheight == 0) {
				for (int k = 0; k < blocklinebyte; k++)
					*(pBmpBuffer + i * linebyte + j * blocklinebyte + k) = 0xFF;
			}

			for (int k = 0; k < 3; k++) {
				*(pBmpBuffer + i * linebyte + (j + 1)*blocklinebyte - 3 + k) = 0xFF;
			}
			if (i > bmpHeight - cutheight && j == m - 1)
				for (int k = 0; k < blocklinebyte - 3; k++)
					*(pBmpBuffer + i * linebyte + j * blocklinebyte + k) = 0xFF;
		}
	//����puzzle�е����д���ͼƬ
	for (int i = n - 1; i >= 0; i--)
		for (int k = 0; k < cutheight; k++) {
			for (int j = 0; j < m; j++) {
				int tmp = puzzle[i*m + j];
				if (tmp == -1)
					tmp = m * n - 1;
				int y = tmp / m;
				int x = tmp % m;
				fwrite(pBmpBuffer + linebyte * (k + y * cutheight) + blocklinebyte * x, 1, blocklinebyte, fout);
			}
		}

	fclose(fout);
	cout << "ͼƬ����ɹ���\n";
	delete[] pBmpBuffer;
	return 1;
}
//�ͷ�ͼƬռ�õĿռ䣬�����ڴ�й©
void Freebmp() {
	if(pBmpBuf)
	delete[] pBmpBuf;
}

//һ��С�ʵ�
void printEgg() {
	cout << "��ʱ��ô�̣������������˰ɣ�\n";
	for (int x = 0; x < 30; x++) {
		for (int y = 0; y < 60 + 0; y++) {
			if (x == 0 || x == 29) { if (y < 0)cout << " "; else cout << "*"; }
			else if (x == 1) { if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else cout << " "; }
			else if (x == 2) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 9 || y == 0 + 50)cout << "*";
				else if (y == 0 + 22 || y == 0 + 24 || y == 0 + 26 || y == 0 + 28 || y == 0 + 30 || y == 0 + 32 || y == 0 + 34 ||
					y == 0 + 36 || y == 0 + 38)cout << "*"; else cout << " ";
			}
			else if (x == 3) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 8 + 0 || y == 10 + 0)cout << "*";
				else if (y == 12 + 0 || y == 18 + 0 || y == 0 + 42 || y == 0 + 47 || y == 0 + 49 || y == 0 + 51)cout << "*"; else cout << " ";
			}
			else if (x == 4) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 8 || y == 0 + 11 || y == 0 + 15 || y == 0 + 45
					|| y == 0 + 48 || y == 0 + 51)cout << "*"; else cout << " ";
			}
			else if (x == 5) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 8 || y == 0 + 13 || y == 0 + 47 || y == 0 + 51)
					cout << "*"; else cout << " ";
			}
			else if (x == 6) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 8 || y == 0 + 51)
					cout << "*"; else cout << " ";
			}
			else if (x == 7) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 6
					|| y == 0 + 53)cout << "*"; else cout << " ";
			}
			else if (x == 8) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 5
					|| y == 0 + 54)cout << "*"; else cout << " ";
			}
			else if (x == 9) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 4
					|| y == 0 + 55)cout << "*"; else cout << " ";
			}
			else if (x == 10) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 3
					|| y == 0 + 56)cout << "*";
				else if (y >= 0 + 9 && y != 0 + 10 && y != 0 + 12 && y != 0 + 14 && y != 0 + 16 && y != 0 + 18 && y != 0 + 20 && y != 0 + 22 && y != 0 + 24 && y != 0 + 26 && y <= 0 + 27)cout << "*";
				else if (y >= 0 + 32 && y != 0 + 33 && y != 0 + 35 && y != 0 + 37 && y != 0 + 39 && y != 0 + 41 && y != 0 + 43 && y != 0 + 45 && y != 0 + 47 && y != 0 + 49 && y <= 0 + 50)cout << "*"; else cout << " ";
			}
			else if (x == 11) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 10 || y == 0 + 49 || y == 0 + 26 || y == 0 + 33)
					cout << "*"; else if (y >= 0 + 14 && y <= 0 + 17)cout << "$"; else if (y >= 0 + 42 &&
						y <= 0 + 45)cout << "*"; else cout << " ";
			}
			else if (x == 12) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 12 || y == 0 + 24 || y == 0 + 35 || y == 0 + 47)
					cout << "*"; else cout << " ";
			}
			else if (x == 13) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y >= 0 + 14 && y != 0 + 15 && y != 0 + 17 && y != 0 + 19 && y <= 0 + 20)cout << "*"; else if
					(y >= 0 + 39 && y != 0 + 40 && y != 0 + 42 && y != 0 + 44 && y <= 0 + 45)cout << "*"; else cout << " ";
			}
			else if (x == 14) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else cout << " ";
			}
			else if (x == 15) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 8 || y == 50 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 16) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 8 || y == 50 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 17) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 8 || y == 50 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 18) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 9 || y == 49 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 19) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 2
					|| y == 0 + 57)cout << "*"; else if (y == 0 + 10 || y == 48 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 20) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 3
					|| y == 0 + 56)cout << "*"; else if (y == 0 + 11 || y == 47 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 21) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 4

					|| y == 0 + 55)cout << "*"; else if (y == 0 + 13 || y == 45 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 22) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 5
					|| y == 0 + 54)cout << "*"; else if (y == 0 + 15 || y == 43 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 23) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 7
					|| y == 0 + 52)cout << "*"; else if (y == 0 + 17 || y == 41 + 0) cout << "*"; else cout << " ";
			}
			else if (x == 24) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 10
					|| y == 0 + 49)cout << "*"; else if (y == 0 + 21 || y == 0 + 25 || y == 0 + 29 || y == 0 + 33 || y == 0 + 37)cout << "*"; else cout << " ";
			}
			else if (x == 25) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 13
					|| y == 0 + 46)cout << "*"; else cout << " ";
			}
			else if (x == 26) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if
					(y >= 0 + 17 && y != 0 + 18 && y != 0 + 20 && y != 0 + 22 && y != 0 + 24 && y != 0 + 26 &&
						y != 0 + 28 && y != 0 + 29 && y != 0 + 31 && y != 0 + 33 && y != 0 + 35 && y != 0 + 37 &&
						y != 0 + 39 && y != 0 + 41 && y <= 42 + 0)cout << "*"; else cout << " ";
			}
			else if (x == 27) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 16
					|| y == 0 + 25 || y == 0 + 29 || y == 0 + 30 || y == 0 + 31 || y == 0 + 35 || y == 0 + 43)cout << "*"; else cout << " ";
			}
			else if (x == 28) {
				if (y < 0)cout << " "; else if (y == 0 || y == 59 + 0) cout << "#"; else if (y == 0 + 15
					|| y == 0 + 26 || y == 0 + 27 || y == 0 + 33 || y == 0 + 34 || y == 0 + 44)cout << "*"; else cout << " ";
			}
		}
		cout <<"\n";
	}
	cout << "�����ڿ�����Ŷ��\n";
}