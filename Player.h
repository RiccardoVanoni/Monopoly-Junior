#pragma once
#include "olcPixelGameEngine.h"

class Casella_Proprietà;

class Player
{
public:
	int pos = 0;
	int soldi = 0;
	bool inPrigione = false;
	
	int passi = 0;

	std::string name;

	std::vector<Casella_Proprietà*> possedute;

private:
	olc::Sprite* spr = nullptr;
	olc::Decal* dec = nullptr;

	olc::vf2d scale;
	olc::vf2d ideale;

public:
	
	Player(std::string nomeSprite, std::string name = "");

	~Player() {
		delete dec;
		delete spr;
	}

	void draw(olc::PixelGameEngine* main, olc::vi2d tableDim);

private:

	olc::vi2d posASchermo(int pos, int cardW, int cardH, olc::vi2d tableDim);
};

