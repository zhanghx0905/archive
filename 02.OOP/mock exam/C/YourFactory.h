#pragma once

#include "AbstractFactory.h"

class HumanFactory: public AbstractFactory
{
public:
	Casern* createCasern() {
		Casern* tmp = new HumanCasern;
		return tmp;
	}
	Footman* createFootman(string name) {
		Footman* tmp = new HumanFootman(name);
		return tmp;
	}
	Commander* createCommander(string name) {
		Commander* tmp = new HumanCommander(name);
		return tmp;
	}
	Belong* createBelong(Footman* _footman, Commander* _commander) {
		HumanFootman* tmp1 = dynamic_cast<HumanFootman*>(_footman);
		HumanCommander* tmp2 = dynamic_cast<HumanCommander*>(_commander);
		Belong* tmp = new HumanBelong(tmp1, tmp2);;
		return tmp;
	}
};

class OrcFactory : public AbstractFactory {
public: 
	Casern* createCasern() {
		Casern* tmp = new OrcCasern;
		return tmp;
	}
	Footman* createFootman(string name) {
		Footman* tmp = new OrcFootman(name);
		return tmp;
	}
	Commander* createCommander(string name) {
		Commander* tmp = new OrcCommander(name);
		return tmp;
	}
	Belong* createBelong(Footman* _footman, Commander* _commander) {
		OrcFootman* tmp1 = dynamic_cast<OrcFootman*>(_footman);
		OrcCommander* tmp2 = dynamic_cast<OrcCommander*>(_commander);
		Belong* tmp = new OrcBelong(tmp1, tmp2);
		return tmp;
	}
};
