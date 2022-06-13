#pragma once
#include "SkilledHero.h"

class BladestormHero : public SkilledHero {
public:
	name2Level store;
	BladestormHero(Hero* h) {
		h->getSkill(store);
		store["Bladestorm"]++;
	}
	void getSkill(name2Level& skillMap) {
		skillMap = store;
	}
};