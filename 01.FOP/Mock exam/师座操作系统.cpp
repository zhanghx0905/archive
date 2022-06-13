/*
问题描述
　　文件系统有的所有文件都有一个独一无二的文件名，除此之外分为两类文件，一类文件是数据存储文件，它可以存储一个字符串信息，另一类文件是快捷方式，它会指向另一个文件，有可能是数据块也有可能是快捷方式。
　　文件系统支持3条命令：
　　1.创建命令：create <FileName> <FileType> <FileInfo>
　　这个命令的意思是，创建一个文件名为<FileName>，文件类型为<FileType>，文件信息为<FileInfo>，文件类型为0或者1,0表示数据块，1表示快捷方式，如果是数据块，那么<FileInfo>表示储存的字符串，如果这是一个快捷方式，<FileInfo>表示指向的文件的名称，如果当前已存在名为<FileName>的文件，则更新这个文件的信息。
　　2.打开命令：open <FileName>
　　这个命令是打开文件名为<FileName>的文件，如果这是一个快捷方式，则会打开这个快捷方式指向的文件，直到打开一个数据块时，显示这个数据块储存的信息并换行。
　　3.退出命令：exit
　　得到这个命令以后，你的程序需要安全终止。
输入格式
　　若干条命令构成，最后一条命令必然为exit。
输出格式
　　输出每次使用open命令的显示结果。
数据规模和约定
　　总命令条数不超过1000条。
　　保证<FileName>，<FileType>，<FileInfo>不包含空格和不合法字符，每个长度不超过20个字符。
*/

#include<iostream>
#include<cstdio>
#include<string>
#include<string.h>
using namespace std;

char order[10], name[25], info[25];
int type, ord;

struct File {
	char name[25];
	char info[25];
	File* toFile;
}Files[1000];

int main() {
	while (true) {
		scanf("%s", order);

		if (!strcmp(order, "exit")) break;

		else if (!strcmp(order, "create")) {
			scanf("%s %d %s", name, &type, info);
			int mark = 0;
			//如果是数据块
			if (!type) {
				//查找同名文件是否已经存在
				for (int i = 0; i < ord; i++)
					if (!strcmp(Files[i].name, name)) {
						strcpy(Files[i].info, info);
						Files[i].toFile = NULL;
						mark = 1;
						break;
					}
				//如果不存在，创建新文件
				if (mark == 0) {
					strcpy(Files[ord].name, name);
					strcpy(Files[ord].info, info);
					Files[ord].toFile = NULL;
					ord++;
				}
			}
			//如果是快捷方式
			else {
				//查找同名文件是否已经存在
				for (int j = 0; j < ord; j++) {
					if (!strcmp(Files[j].name, name)) {
						//如果已经存在，再查找快捷方式指向的文件
						mark = 1;
						for (int i = 0; i < ord; i++) {
							if (j != i && !strcmp(Files[i].name, info)) {
								Files[j].toFile = &Files[i];
								//Files[j].info[0] = '\0';
								break;
							}
						}
					}
				}
				//如果不存在，创建新快捷方式
				if (mark == 0) {
					for (int i = 0; i < ord; i++) {
						if (!strcmp(Files[i].name, info)) {
							strcpy(Files[ord].name, name);
							Files[ord].toFile = &Files[i];
							ord++;
							break;
						}
					}
				}
			}
		}

		else if (!strcmp(order, "open")) {
			scanf("%s", name);
			for (int i = 0; i < ord; i++) {
				if (!strcmp(Files[i].name, name)) {
					File* Files_p = &Files[i];
					while (Files_p->toFile != NULL) Files_p = Files_p->toFile;
					cout << Files_p->info << endl;
					break;
				}
			}
		}
	}

	return 0;
}