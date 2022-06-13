#pragma once
#include "SkilledHero.h"

class AvatarHero : public SkilledHero {
public:
	AvatarHero(Hero* h) {
		h->getSkill(store);
		store["Avatar"]++;
	}
	void getSkill(name2Level& skillMap) {
		skillMap = store;
	}
	name2Level store;
};