#pragma once
#include "Player.h"

class Carta {
public:
	std::string testo;
	Carta(std::string testo) : testo(testo) {};

public:
	virtual bool azione(std::vector<Player*> v, int currentPlayer) = 0;
};

//aumenta soldi di due
class Carta_1 : public Carta {
public:
	Carta_1() 
		: Carta("E' il tuo compleanno! Prendi due euro dalla banca") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) override{
		players[currentPlayer]->soldi += 2;

		return false;
	}
};

//diminuisce soldi di due
class Carta_2 : public Carta {
public:
	Carta_2() 
		: Carta("Paga le tasse e dai due euro alla banca") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) override {
		players[currentPlayer]->soldi -= 2;

		return false;
	}
};

//vai ultima casella da ricchi
class Carta_3 : public Carta {
public:
	Carta_3() : Carta("Vai fino a parco della vittoria") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) override {
		players[currentPlayer]->pos = 23;

		return true;
	}
};

//vai in prigione senza passare dal via
class Carta_4 : public Carta {
public:
	Carta_4() : Carta("Vai in prigione senza passare dal via") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) override {
		players[currentPlayer]->inPrigione = true;
		players[currentPlayer]->pos = 6;

		return false;
	}
};

//vai al negozio di animali
class Carta_5 : public Carta {
public:
	Carta_5() : Carta("Vai fino al negozio di animali") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) {
		players[currentPlayer]->pos = 17;
		
		return true;
	}
};

//perdi tre e li dai al tipo dopo di te
class Carta_6 : public Carta {
public:
	Carta_6() : Carta("Dai dieci al tipo dopo di te") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer){

		players[currentPlayer]->soldi -= 3;
		if (currentPlayer == players.size() - 1) {
			players[0]->soldi += 3;
		}
		else {
			players[currentPlayer + 1]->soldi += 3;
		}

		return false;
	}
};

//vai al via
class Carta_7 : public Carta {
public:
	Carta_7() : Carta("Vai avanti fino al via") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) {
		players[currentPlayer]->pos = 0;
		players[currentPlayer]->soldi += 2;

		return false;
	}
};

//scambi di posizione con il giocatore dopo di te
class Carta_8 : public Carta {
public:
	Carta_8()
		: Carta("Scambia la tua posizione con il tipo dopo di te") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) {

		int nextPlayer = currentPlayer + 1;
		if (currentPlayer == players.size() - 1)  nextPlayer = 0;

		int tmp = players[nextPlayer]->pos;
		players[nextPlayer]->pos = players[currentPlayer]->pos;
		players[currentPlayer]->pos = tmp;
		
		return false;
	}
};

//dai 3 al giocatore prima di te
class Carta_9 : public Carta {
public:
	Carta_9() : Carta("Dai tre al giocatore prima di te") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) {

		players[currentPlayer]->soldi -= 3;
		if (currentPlayer == 0) players[players.size() - 1]->soldi += 3;
		else players[currentPlayer - 1]->soldi += 3;

		return false;
	}
};

//vai al parcheggio senza passare dal via
class Carta_10 : public Carta {
public:
	Carta_10() : Carta("Vai al parcheggio senza passare dal via") {};

public:
	virtual bool azione(std::vector<Player*> players, int currentPlayer) {
		players[currentPlayer]->pos = 12;

		return false;
	}
};

class Carta_11 : public Carta {
public:
	Carta_11() : Carta("paga 2 soldi alla banca") {};
public:
	virtual bool azione(std::vector<Player*> v, int currentPlayer) override {
		//IMPREVISTO: paga 2 soldi alla banca
		v[currentPlayer]->soldi -= 2;

		return false;
	}

};

class Carta_12 : public Carta {
public:
	Carta_12() : Carta("vai fino a \"viale dei Giardini\" e paga il passaggio") {};
public:
	virtual bool azione(std::vector<Player*> v, int currentPlayer) override {
		//IMPREVISTO: vai fio a "viale dei Giardini" e paga il passaggio
		v[currentPlayer]->pos = 22;
		v[currentPlayer]->soldi -= 2;

		return false;
	}

};

class Carta_13 : public Carta {
public:
	Carta_13() : Carta("casella gratuita: usala per andare al bowling. entri gratuitamente") {};
public:
	virtual bool azione(std::vector<Player*> v, int currentPlayer) override {
		//IMPREVISTO: casella gratuita: usala per andare al bowling. entri gratuitamente
		v[currentPlayer]->pos = 19;

		return true;
	}

};
