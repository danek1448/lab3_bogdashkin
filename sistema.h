#ifndef SISTEMA_H
#define SISTEMA_H

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include "truba.h"
#include "cs.h"
#include "logirovanie.h"
#include "gazoset.h"

using namespace std;

class Sistema {
private:
    Gazoset gazoset;
    int sleduyushiy_id_truba;
    int sleduyushiy_id_ks;

public:
    Sistema();
    void Dobavit_trubu();
    void Dobavit_KS();
    void Pokazat_vse_obekty() const;
    void Redaktirovat_trubu();
    void Redaktirovat_KS();
    void Udalit_trubu();
    void Udalit_KS();
    void Poisk_trub();
    void Poisk_KS();
    void Paketnoe_redaktirovanie_trub();
    void Sohranit_dannye();
    void Zagruzit_dannye();

    void Soedinit_KS();
    void Pokazat_soedineniya() const;
    void Udalit_soedinenie();
    void Topologicheskaya_sortirovka();
    void Pokazat_gazoset() const;  

    void CalculateMaxFlow();
    void FindShortestPath();
};

#endif