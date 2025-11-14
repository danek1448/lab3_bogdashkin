#ifndef SISTEMA_H
#define SISTEMA_H

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "truba.h"
#include "cs.h"
#include "logirovanie.h"

using namespace std;

class Sistema {
private:
    unordered_map<int, Truba> truby;
    unordered_map<int, Kompressornaya_stantsiya> kompressornye_stantsii;
    int sleduyushiy_id_truba;
    int sleduyushiy_id_ks;

public:
    Sistema();
    void Dobavit_trubu();
    void Dobavit_KS();
    void Pokazat_vse_obekty() const;
    void Redaktirovat_trubu();
    void Redaktirovat_KS();
    void Poisk_trub();
    void Poisk_KS();
    void Paketnoe_redaktirovanie_trub();
    void Sohranit_dannye();
    void Zagruzit_dannye();
};

#endif