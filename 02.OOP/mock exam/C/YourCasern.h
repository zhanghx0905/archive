#pragma once

#include "Footman.h"
#include "Commander.h"
#include "Belong.h"
#include "Casern.h"

class HumanFootman :  public Footman {
public:
	HumanFootman(string name) :Footman(name) {};
};

class OrcFootman :  public Footman {
public:
	OrcFootman(string name) :Footman(name) {};
};

class HumanCommander :  public Commander {
public:
	HumanCommander(string name) :Commander(name) {};
};

class OrcCommander :  public Commander {
public:
	OrcCommander(string name) :Commander(name) {};
};

class HumanBelong :  public Belong {
public:
	HumanBelong(HumanFootman* _footman, HumanCommander* _commander) :
		Belong(_footman, _commander) {};
};

class OrcBelong :  public Belong {
public:
	OrcBelong(OrcFootman* _footman, OrcCommander* _commander) :
		Belong(_footman, _commander) {};
};

class HumanCasern : public Casern {
public:
	string getKind() {
		return "HumanCasern";
	}
};

class OrcCasern : public Casern {
public:
	string getKind() {
		return "OrcCasern";
	}
};
