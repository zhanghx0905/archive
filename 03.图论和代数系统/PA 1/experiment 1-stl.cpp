#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <fstream>
using namespace std;

class digraph
{
private:
	int dots;   //点的个数
	int edges;  //边的个数

	vector< list<int> > adTable;  //邻接表
	vector<int> edgeList[3];      //边列表
	vector <vector<int> > InMatrix;//关联矩阵
	vector <vector<int> > wMatrix;//权矩阵
	struct Positive_Table {//正向表
		list<int> index;   //记录结点的第一个直接后继在sub中的下标位置
		list<pair<int, int> > sub;  //记录相应直接后继的编号和相应的权值
	}PoTable;

	void Inigraph();
public:
	digraph(int _dots);
	//~digraph();
	friend ostream& operator<<(ostream& fout, const digraph& sr);
	friend istream& operator>>(istream& in, digraph& sr);
};

istream& operator>>(istream& in, digraph& sr);
ostream& operator<<(ostream& fout, const digraph& sr);

void digraph::Inigraph()
{
	for (int i = 0; i < 3; i++)
		edgeList[i].resize(edges + 1);
	adTable.resize(dots + 1);
	InMatrix.resize(dots + 1);
	for (int i = 0; i < InMatrix.size(); i++)
		InMatrix[i].resize(edges + 1);

	int k = 1, tmp = 0;
	for (int i = 1; i <= dots; i++) {
		PoTable.index.push_back(k);
		for (int j = 1; j <= dots; j++) {
			tmp = wMatrix[i][j];
			if (tmp != 0) {
				InMatrix[i][k] = 1;
				InMatrix[j][k] = -1;

				edgeList[0][k] = i;
				edgeList[1][k] = j;
				edgeList[2][k] = tmp;

				adTable[i].push_back(tmp);
				adTable[i].push_back(j);

				PoTable.sub.push_back(make_pair(j, tmp));

				k++;
			}
		}
	}
	PoTable.index.push_back(k);
}

digraph::digraph(int _dots = 0) :dots(_dots), edges(0)
{

}


/*
digraph::~digraph()
{
}*/

ostream & operator<<(ostream & out, const digraph & sr)
{
	//out << "关联矩阵:" << endl;
	for (int i = 1; i <= sr.dots; i++) {
		for (int j = 1; j <= sr.edges; j++) {
			out << sr.InMatrix[i][j] << " ";
		}
		out << endl;
	}

	//out << "边列表:" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 1; j <= sr.edges; j++) {
			out << sr.edgeList[i][j] << " ";
		}
		out << endl;
	}

	//out << "正向表:" << endl;
	for (list<int>::const_iterator iter = sr.PoTable.index.begin(); iter != sr.PoTable.index.end(); iter++) {
		out << *iter << " ";
	}
	out << endl;
	for (list<pair<int, int> >::const_iterator iter = sr.PoTable.sub.begin(); iter != sr.PoTable.sub.end(); iter++) {
		out << (*iter).first << " ";
	}
	out << endl;
	for (list<pair<int, int> >::const_iterator iter = sr.PoTable.sub.begin(); iter != sr.PoTable.sub.end(); iter++) {
		out << (*iter).second << " ";
	}
	out << endl;


	//out << "邻接表:" << endl;
	for (vector< list<int> >::const_iterator iter_0 = sr.adTable.begin() + 1; iter_0 != sr.adTable.end(); iter_0++) {
		for (list<int>::const_iterator iter_1 = (*iter_0).begin(); iter_1 != (*iter_0).end(); iter_1++) {
			out << *iter_1 << " ";
		}
		out << endl;
	}
	return out;
}

istream & operator>>(istream & in, digraph & sr)
{
	int tmp = 0;
	sr.wMatrix.resize(sr.dots + 1);
	for (int i = 1; i <= sr.dots; i++) {
		sr.wMatrix[i].resize(sr.dots + 1);
		for (int j = 1; j <= sr.dots; j++) {
			in >> tmp;
			sr.wMatrix[i][j] = tmp;
			if (tmp != 0) sr.edges++;
		}
	}
	sr.Inigraph();

	return in;
}

int main() {
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	int dots = 0;
	fin >> dots;
	if (0 < dots && dots < 100) {
		digraph graph(dots);
		fin >> graph;

		fout << graph;
	}
	fin.close();
	fout.close();

	return 0;
}