#include <iostream>
#include <vector>
#include <list>
#include <fstream>
using namespace std;
const int dots_max = 100, edges_max = 5000;  //标记点和边的最大值

int dots;   //点的个数
int edges;  //边的个数
int InMatrix[dots_max + 1][edges_max + 1];//关联矩阵,每一列的第0行记录权值
int edgeList[3][edges_max + 1];           //边列表
struct Positive_Table {//正向表
	int index[dots_max + 2];      //记录结点v_i的第一个直接后继在sub中的下标位置
	int sub[2][edges_max + 1];//记录相应直接后继的编号和相应的权值
}PoTable;

void Ini_Digraph(vector< list<int> >&, ifstream& fin);      //构造函数
void printAll(vector< list<int> >, ofstream& fout);            //打印所有矩阵
void Edgelist_to_adTable(vector< list<int> >&);   //边列表转邻接表
void Edgelist_to_InMatrix();  //边列表转关联矩阵
void Edgelist_to_poTable();   //边列表转正向表


int main() {
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	fin >> dots;
	if (0 < dots && dots < 100) {
		vector< list<int> > adTable(dots + 1); //邻接表
		Ini_Digraph(adTable, fin);
		printAll(adTable, fout);
	}
	fin.close();
	fout.close();
	return 0;
}

void Ini_Digraph(vector< list<int> >& adTable, ifstream& fin) {
	int temp = edges = 0;
	for (int i = 1; i <= dots; i++)
		for (int j = 1; j <= dots; j++) {
			fin >> temp;
			if (temp != 0) {
				edges++;
				edgeList[0][edges] = i;
				edgeList[1][edges] = j;
				edgeList[2][edges] = temp;
			}
		}
	Edgelist_to_InMatrix();
	Edgelist_to_poTable();
	Edgelist_to_adTable(adTable);
}

void printAll(vector< list<int> > adTable, ofstream& fout)
{
	//fout << "关联矩阵:" << endl;
	for (int i = 1; i <= dots; i++) {
		for (int j = 1; j <= edges; j++) {
			fout << InMatrix[i][j];
			if (j != edges)
				fout << " ";
		}
		fout << endl;
	}

	//fout << "边列表:" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 1; j <= edges; j++) {
			fout << edgeList[i][j];
			if (j != edges)
				fout << " ";
		}
		fout << endl;
	}

	//fout << "正向表:" << endl;
	for (int i = 1; i <= dots + 1; i++) {
		fout << PoTable.index[i];
		if (i != dots + 1)
			fout << " ";
	}
	fout << endl;
	for (int i = 0; i < 2; i++) {
		for (int j = 1; j <= edges; j++) {
			fout << PoTable.sub[i][j];
			if (j != edges)
				fout << " ";
		}
		fout << endl;
	}

	//fout << "邻接表:" << endl;
	for (vector< list<int> >::iterator iter_0 = adTable.begin() + 1; iter_0 != adTable.end(); iter_0++) {
		int mark = 0;
		for (list<int>::iterator iter_1 = (*iter_0).begin(); iter_1 != (*iter_0).end(); iter_1++) {
			if (!mark) {
				mark = 1;
				fout << *iter_1;
			}
			else
				fout << " " << *iter_1;
		}
		if (iter_0 + 1 != adTable.end())fout << endl;
	}
}

void Edgelist_to_InMatrix()
{
	for (int i = 1; i <= edges; i++) {
		InMatrix[edgeList[0][i]][i] = 1;
		InMatrix[edgeList[1][i]][i] = -1;
		InMatrix[0][i] = edgeList[2][i];
	}
}

void Edgelist_to_poTable()
{
	int j = 1;
	for (int i = 1; i <= edges + 1; ) {
		PoTable.index[j] = i;
		while (edgeList[0][i] == j && i <= edges + 1) {
			PoTable.sub[0][i] = edgeList[1][i];
			PoTable.sub[1][i] = edgeList[2][i];
			i++;
		}
		j++;
		if (j > dots) {
			PoTable.index[j] = i;
			break;
		}
	}
}

void Edgelist_to_adTable(vector< list<int> >& adTable) {
	int j = 1;

	for (int i = 1; i <= edges; ) {
		while (edgeList[0][i] == j && i <= edges) {
			adTable[j].push_back(edgeList[2][i]);
			adTable[j].push_back(edgeList[1][i]);
			i++;
		}
		j++;
	}
}