#pragma once
#include "Footman.h"
#include "Commander.h"
#include "Belong.h"
#include <vector>
using namespace std;

class Casern {
	vector<Footman*> footmen;
	vector<Commander*> commanders;
	vector<Belong*> belongs;
public:
	virtual std::string getKind() = 0;
	virtual void addFootman(Footman* footman) {
		footmen.push_back(footman);
	}
	virtual void addCommander(Commander* command) {
		commanders.push_back(command);
	}
	virtual void addBelong(Belong* belong) {
		belongs.push_back(belong);
	}
	virtual Footman* getFootmanbyIndex(int n) {
		if (n < footmen.size())
			return footmen[n];
		return nullptr;
	}
	virtual Commander* getCommanderbyIndex(int n) {
		if (n < commanders.size())
			return commanders[n];
		return nullptr;
	}
	virtual Belong* getBelongbyIndex(int n) {
		if (n < belongs.size())
			return belongs[n];
		return nullptr;
	}
};