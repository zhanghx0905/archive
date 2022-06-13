#include "ComputerCollection.h"
#include <cstring>
#include <algorithm>
using namespace std;
//�ı�set�����ֵ��set�����Զ����������Ҫ���³�ʼ��set
void ComputerCollection::sortByScore()
{
	set<Computer> tmp;
	for (set<Computer>::iterator i = comp.begin(); i != comp.end(); i++)
		tmp.insert(*i);
	comp = tmp;
}

Computer & ComputerCollection::operator[](const string &str)
{//Ҫ��comp[i]�����޸Ĳ���������ע�ⷵ������

	set<Computer>::iterator i;
	for (i = comp.begin(); i != comp.end(); i++)
		if (str == (*i).getName())
			return const_cast<Computer&>(*i);
}

vector<string> split(const string &str, const string &sep)
{
	vector<string> tmp;
	string::size_type pos[2] = { 0, str.find(sep) };
	while (string::npos != pos[1])
	{
		tmp.push_back(str.substr(pos[0], pos[1] - pos[0]));
		pos[0] = pos[1] + sep.size();
		pos[1] = str.find(sep, pos[0]);
	}
	if (pos[0] != str.length())
		tmp.push_back(str.substr(pos[0]));

	return tmp;
}

istream & operator>>(istream &in, ComputerCollection &sc)
{
	string str;
	in >> str;
	vector<string> strs = split(str, "-");

	Computer cmp = Computer(atoi(strs[2].c_str()), atoi(strs[1].c_str()), strs[0]);
	sc.comp.insert(cmp);

	return in;
}

ostream & operator<<(ostream &out, const ComputerCollection &sc)
{
	for (set<Computer>::const_reverse_iterator i=sc.comp.crbegin(); i !=sc.comp.crend(); i++)
		out << *i<<endl;
	return out;
}