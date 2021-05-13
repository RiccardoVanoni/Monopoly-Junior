#include "Player.h"

#include "Casella.h"

Player::Player(std::string nomeSprite, std::string name)
	: name(name) {
	spr = new olc::Sprite("./sprites/" + nomeSprite);
	dec = new olc::Decal(spr);

	ideale = { 32,32 };
	scale = { ideale.x / spr->width, ideale.y / spr->height };
}

void Player::draw(olc::PixelGameEngine* main, olc::vi2d tableDim){
	int cardW = 80;
	int cardH = 100;

	olc::vi2d p = posASchermo(pos, cardW, cardH, tableDim);

	if (pos == 6) {
		if (inPrigione) p += { cardH / 10, -cardH / 10 };
		else p -= { 3 * cardH / 10, -3 * cardH / 10 };
	}

	p -= ideale / 2;
	main->DrawDecal(p, dec, scale);

	for (Casella_Proprietà* prop : possedute) {
		olc::vi2d propPos = posASchermo(prop->pos, cardW, cardH, tableDim);

		if (prop->pos < 6) {
			propPos -= { 0, cardH / 2 };
			propPos.x -= ideale.x / 4;
		}
		else if (prop->pos < 12) {
			propPos += { cardH / 2, 0 };

			propPos.x -= ideale.x / 2;
			propPos.y -= ideale.y / 4;
		}
		else if (prop->pos < 18) {
			propPos += { 0, cardH / 2 };

			propPos.x -= ideale.x / 4;
			propPos.y -= ideale.y / 2;
		}
		else {
			propPos -= { cardH / 2, 0 };
			propPos.y -= ideale.y / 4;
		}

		main->DrawDecal(propPos, dec, scale / 2);
	}
}

olc::vi2d Player::posASchermo(int pos, int cardW, int cardH, olc::vi2d tableDim)
{
	olc::vi2d p{ 0,0 };

	if (pos <= 6) p.y = tableDim.y - cardH / 2;
	else if (pos < 12) p.y = tableDim.y - cardH - cardW * (pos - 7) - cardW / 2;
	else if (pos <= 18) p.y = cardH / 2;
	else p.y = cardH + cardW / 2 + cardW * (pos - 19);

	if (pos == 0 || pos >= 18) p.x = tableDim.x - cardH / 2;
	else if (pos < 6) p.x = tableDim.x - cardH - cardW / 2 - cardW * (pos - 1);
	else if (pos <= 12) p.x = cardH / 2;
	else if (pos < 18) p.x = cardH + cardW / 2 + cardW * (pos - 13);

	return p;
}
