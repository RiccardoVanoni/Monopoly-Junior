#pragma once

#include "Player.h"

class Casella {
public:
	int pos;

public:
	Casella(int pos)
		: pos(pos) { }

	virtual int azione(Player* p) = 0;

};

class Casella_ParcheggioGratuito : public Casella {
public:
	Casella_ParcheggioGratuito(int pos)
		: Casella(pos) {

	}

public:
	virtual int azione(Player* p) { return 0; }
};

class Casella_VaiInPrigione : public Casella {
public:
	Casella_VaiInPrigione(int pos) : Casella(pos) {}

public:
	virtual int azione(Player* p) {
		p->inPrigione = true;
		p->pos = 6;
		return 0;
	}
};

class Casella_Proprietà : public Casella {
public:
	int prezzo = 1;
	Player* proprietario = nullptr;
	enum COLORE {
		MARRONE = 0, AZZURRO = 1, MAGENTA = 2, ARANCIO = 3, ROSSO = 4, GIALLO = 5, VERDE = 6, BLU = 7
	} colore;

	Casella_Proprietà(int pos, int prezzo, COLORE colore)
		: Casella(pos), prezzo(prezzo), colore{ colore }{ }

public:
	virtual int azione(Player* p) {
		if (proprietario == nullptr)
			return 1;

		if (proprietario != p) {
			int pagamento = prezzo;

			for (Casella_Proprietà* prop : proprietario->possedute) {
				if (prop != this && prop->colore == this->colore) {
					pagamento *= 2;
					break;
				}
			}

			p->soldi -= pagamento;
			proprietario->soldi += pagamento;
		}


		return 0;
	}
};

class Casella_Imprevisto : public Casella {
public:
	Casella_Imprevisto(int pos) : Casella(pos) {}
public:
	virtual int azione(Player* p){
		return 2;
	}
};