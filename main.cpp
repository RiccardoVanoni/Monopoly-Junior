#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "Player.h"
#include "Casella.h"
#include "Carta.h"

#include <vector>
#include <algorithm>
#include <random>

class Monopoli : public olc::PixelGameEngine
{
public:
	Monopoli()
	{
		sAppName = "Monopoli";
	}

private:
	olc::Sprite* sprBoard;
	olc::Decal* decBoard;
	enum states { INIZIO_PARTITA, TIRO, MUOVI, EFFETTO_CASELLA, PASSA_TURNO, COMPRA, IMPREVISTI, GAMEOVER } currentState, nextState;

	std::vector<Player*> players;
	int currentPlayer = 0;

	std::vector<Casella*> board;
	std::vector<Carta*> carte;

	float tempo = 0;

	std::vector<std::string> text;

public:
	bool OnUserCreate() override
	{
		sprBoard = new olc::Sprite("./sprites/board.jpg");
		decBoard = new olc::Decal(sprBoard);

		currentState = INIZIO_PARTITA;
		nextState = INIZIO_PARTITA;

		srand(time(NULL));

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		////////////////LOGICA///////////
		tempo += fElapsedTime;

		float tempoPerFrame = 0.5f;
		if (GetKey(olc::T).bHeld) tempoPerFrame = 0.05f;

		bool timer = false;
		if (tempo > tempoPerFrame) {
			tempo = 0;
			timer = true;
		}

		switch (currentState) {
		case INIZIO_PARTITA:

			///////////TABELLONE////////////
			board.resize(24);
			for (int i = 0; i < 22; i++)
				if (i % 3 != 0)
					board[i] = new Casella_Proprietà(i, i / 6 + 1, Casella_Proprietà::COLORE(i / 3));
				else if (i % 6 != 0)
					board[i] = new Casella_Imprevisto(i);//imprevisti

			board[22] = new Casella_Proprietà(22, 5, Casella_Proprietà::BLU);
			board[23] = new Casella_Proprietà(23, 5, Casella_Proprietà::BLU);
			board[0] = new Casella_ParcheggioGratuito(0);//via
			board[6] = new Casella_ParcheggioGratuito(6);//prigione
			board[12] = new Casella_ParcheggioGratuito(12);//parcheggio gratuito
			board[18] = new Casella_VaiInPrigione(18);//vai in prigione

			/////////////////IMPREVISTI////////////

			carte.push_back(new Carta_1());
			carte.push_back(new Carta_2());
			carte.push_back(new Carta_3());
			carte.push_back(new Carta_4());
			carte.push_back(new Carta_5());
			carte.push_back(new Carta_6());
			carte.push_back(new Carta_7());
			carte.push_back(new Carta_8());
			carte.push_back(new Carta_9());
			carte.push_back(new Carta_10());
			carte.push_back(new Carta_11());
			carte.push_back(new Carta_12());
			carte.push_back(new Carta_13());

			//std::shuffle(carte.begin(), carte.end(), std::default_random_engine());


			//////////GIOCATORI//////////////
			players.push_back(new Player("fox.png", "Volpe"));
			players.push_back(new Player("cat.png", "Gatto"));
			players.push_back(new Player("dog.png", "Cane"));
			players.push_back(new Player("zebra.png", "Zebra"));

			for (Player* p : players) p->soldi = 16;

			currentPlayer = 0;

			nextState = TIRO;

			break;

		case TIRO: {

			int tiro = (rand() % 6) + 1;
			players[currentPlayer]->passi = tiro;

			nextState = MUOVI;
			break;
		}

		case MUOVI:
			if (timer) {
				players[currentPlayer]->passi--;
				players[currentPlayer]->pos++;
				if (players[currentPlayer]->pos > 23) {
					players[currentPlayer]->pos = 0;

					players[currentPlayer]->soldi += 2; //passaggio dal via
				}

				if (players[currentPlayer]->passi <= 0) nextState = EFFETTO_CASELLA;
			}

			break;

		case EFFETTO_CASELLA: {
			int n = board[players[currentPlayer]->pos]->azione(players[currentPlayer]);

			if (n == 1) nextState = COMPRA;
			else if (n == 2) nextState = IMPREVISTI;
			else nextState = PASSA_TURNO;

			break;
		}

		case PASSA_TURNO:
			currentPlayer++;
			if (currentPlayer >= players.size()) currentPlayer = 0;
			if (players[currentPlayer]->inPrigione) {
				players[currentPlayer]->soldi--;
				setText(players[currentPlayer]->name + " e' uscito di prigione");
			}

			players[currentPlayer]->inPrigione = false;

			nextState = TIRO;

			break;

		case COMPRA: {
			Casella_Proprietà* prop = dynamic_cast<Casella_Proprietà*>(board[players[currentPlayer]->pos]);

			if (prop != nullptr) {
				players[currentPlayer]->soldi -= prop->prezzo;
				players[currentPlayer]->possedute.push_back(prop);
				prop->proprietario = players[currentPlayer];
			}
			nextState = PASSA_TURNO;

			break;
		}
		case IMPREVISTI: {
			if (timer) {
				int idx = rand() % carte.size();

				if (carte[idx]->azione(players, currentPlayer)) nextState = EFFETTO_CASELLA;
				else nextState = PASSA_TURNO;
				//std::cout << idx << "\n";
				nextState = PASSA_TURNO;

				std::vector<std::string> testo;
				std::vector<std::string> testocarta = wrapText(carte[idx]->testo, 30);

				testo.push_back(players[currentPlayer]->name + " ha avuto un IMPREVISTO:");
				testo.insert(testo.end(), testocarta.begin(), testocarta.end());

				setText(testo);
			}
			break;
		}
		case GAMEOVER: {

			break;
		}
		}

		for (Player* p : players)
			if (p->soldi < 0) {
				std::vector<std::string>  classifica;
				classifica.push_back("Partita terminata! La classifica e':");

				std::sort(players.begin(), players.end(), [](auto a, auto b) -> bool {
					return a->soldi > b->soldi;
					});

				for (int i = 0; i < players.size(); i++)
					classifica.push_back(std::to_string(i + 1) + "_ " + players[i]->name + " : " + std::to_string(players[i]->soldi) + "$");

				setText(classifica);

				nextState = GAMEOVER;

				break;
			}

		currentState = nextState;


		////////GRAFICA//////////////////
		Clear(olc::WHITE);

		//olc::vf2d scale = { (float)ScreenWidth() / (float)sprBoard->width, (float)ScreenHeight() / (float)sprBoard->height };
		DrawDecal({ 0,0 }, decBoard);

		float startX = sprBoard->width / 2 - 65;
		float startY = sprBoard->height / 2 - 10 * (int)players.size() + 5;

		if (currentState != GAMEOVER) {

			FillRectDecal({ startX, startY }, { 130.0f,(float)20 * (int)players.size() + 10 }, olc::WHITE);

			for (int i = 0; i < players.size(); i++) {
				players[i]->draw(this, { sprBoard->width, sprBoard->height });

				DrawStringDecal({ startX + 10.0f, startY + 10.0f + 20.0f * i },
					players[i]->name + " : " + std::to_string(players[i]->soldi) + "$", olc::DARK_GREEN);
			}
		}

		displayText();

		return true;
	}


private:

	void setText(std::string s) {
		text = wrapText(s, 30);
	}

	void setText(std::vector<std::string> s) {
		text = s;
	}

	void clearText() {
		text.empty();
	}

	void displayText() {
		if (text.size() == 0) return;

		float startX = sprBoard->width / 2 - 150;
		float startY = 2 * sprBoard->height / 3 - (float)10 * text.size() + 20;

		FillRectDecal({ startX, startY }, { 300.0f, (float)20 * text.size() + 10 }, olc::WHITE);

		for (int i = 0; i < text.size(); i++)
			DrawStringDecal({ startX + 10.0f, startY + (float)20 * i + 10.0f }, text[i], olc::DARK_GREEN);
	}

	std::vector<std::string> wrapText(std::string input, int width) {
		size_t curpos = 0;
		size_t nextpos = 0;

		std::vector<std::string> lines;
		std::string substr = input.substr(curpos, width + 1);

		while (substr.length() == width + 1 && (nextpos = substr.rfind(' ')) != input.npos) {
			lines.push_back(input.substr(curpos, nextpos));
			curpos += nextpos + 1;
			substr = input.substr(curpos, width + 1);
		}

		if (curpos != input.length())
			lines.push_back(input.substr(curpos, input.npos));

		return lines;
	}
};


int main()
{
	Monopoli game;
	if (game.Construct(600, 600, 1, 1))
		game.Start();

	return 0;
}