// kombinacii.cpp
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <algorithm>
#include "kombinacii.h"

using namespace std;

bool sravnitKarty(const karta& a, const karta& b) {
    return a.znach > b.znach;
}

// ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ (не видна извне)
RezultatKombinacii opredelitKombinaciyu(karta komb[5]) {
    karta sorted[5];
    for (int i = 0; i < 5; i++) sorted[i] = komb[i];
    sort(sorted, sorted + 5, sravnitKarty);

    bool flash = true;
    for (int i = 1; i < 5; i++) {
        if (sorted[i].mast != sorted[0].mast) {
            flash = false;
            break;
        }
    }

    bool strit = true;
    for (int i = 0; i < 4; i++) {
        if (sorted[i].znach - sorted[i + 1].znach != 1) {
            strit = false;
            break;
        }
    }

    bool nizkiyStrit = false;
    if (!strit && sorted[0].znach == 14 && sorted[1].znach == 5 &&
        sorted[2].znach == 4 && sorted[3].znach == 3 && sorted[4].znach == 2) {
        nizkiyStrit = true;
        strit = true;
    }

    int chastota[15] = { 0 };
    for (int i = 0; i < 5; i++) {
        if (sorted[i].znach >= 2 && sorted[i].znach <= 14) {
            chastota[sorted[i].znach]++;
        }
    }

    RezultatKombinacii rez;
    memset(rez.znacheniya, 0, sizeof(rez.znacheniya));

    if (flash && strit && !nizkiyStrit && sorted[0].znach == 14) {
        rez.tip = 9;
        rez.znacheniya[0] = 14;
        return rez;
    }
    if (flash && strit) {
        rez.tip = 8;
        rez.znacheniya[0] = nizkiyStrit ? 5 : sorted[0].znach;
        return rez;
    }

    for (int z = 14; z >= 2; z--) {
        if (chastota[z] == 4) {
            rez.tip = 7;
            rez.znacheniya[0] = z;
            for (int i = 0; i < 5; i++) {
                if (sorted[i].znach != z) {
                    rez.znacheniya[1] = sorted[i].znach;
                    break;
                }
            }
            return rez;
        }
    }

    int set = 0, para = 0;
    for (int z = 14; z >= 2; z--) {
        if (chastota[z] == 3) set = z;
        else if (chastota[z] == 2) para = z;
    }
    if (set && para) {
        rez.tip = 6;
        rez.znacheniya[0] = set;
        rez.znacheniya[1] = para;
        return rez;
    }
    if (flash) {
        rez.tip = 5;
        for (int i = 0; i < 5; i++) rez.znacheniya[i] = sorted[i].znach;
        return rez;
    }
    if (strit) {
        rez.tip = 4;
        rez.znacheniya[0] = nizkiyStrit ? 5 : sorted[0].znach;
        return rez;
    }
    if (set) {
        rez.tip = 3;
        rez.znacheniya[0] = set;
        int idx = 1;
        for (int i = 0; i < 5; i++) {
            if (sorted[i].znach != set) {
                rez.znacheniya[idx++] = sorted[i].znach;
            }
        }
        return rez;
    }

    int p1 = 0, p2 = 0, kicker = 0;
    for (int z = 14; z >= 2; z--) {
        if (chastota[z] == 2) {
            if (!p1) p1 = z;
            else p2 = z;
        }
        else if (chastota[z] == 1) {
            kicker = z;
        }
    }
    if (p1 && p2) {
        rez.tip = 2;
        rez.znacheniya[0] = p1;
        rez.znacheniya[1] = p2;
        rez.znacheniya[2] = kicker;
        return rez;
    }
    if (p1) {
        rez.tip = 1;
        rez.znacheniya[0] = p1;
        int idx = 1;
        for (int i = 0; i < 5; i++) {
            if (sorted[i].znach != p1) {
                rez.znacheniya[idx++] = sorted[i].znach;
            }
        }
        return rez;
    }

    rez.tip = 0;
    for (int i = 0; i < 5; i++) rez.znacheniya[i] = sorted[i].znach;
    return rez;
}

// ОСНОВНЫЕ ФУНКЦИИ (они объявлены в .h)
RezultatKombinacii naitiLuchshuyuKombinaciyu(karta ruka[2], karta stol[5]) {
    karta vse[7];
    int n = 0;
    for (int i = 0; i < 2; i++) {
        if (ruka[i].znach != 0) vse[n++] = ruka[i];
    }
    for (int i = 0; i < 5; i++) {
        if (stol[i].znach != 0) vse[n++] = stol[i];
    }

    RezultatKombinacii luchshaya;
    bool est = false;

    for (int a = 0; a < n; a++) {
        if (vse[a].znach == 0) continue;
        for (int b = a + 1; b < n; b++) {
            if (vse[b].znach == 0) continue;
            for (int c = b + 1; c < n; c++) {
                if (vse[c].znach == 0) continue;
                for (int d = c + 1; d < n; d++) {
                    if (vse[d].znach == 0) continue;
                    for (int e = d + 1; e < n; e++) {
                        if (vse[e].znach == 0) continue;

                        karta komb[5] = { vse[a], vse[b], vse[c], vse[d], vse[e] };
                        RezultatKombinacii tek = opredelitKombinaciyu(komb);

                        if (!est ||
                            tek.tip > luchshaya.tip ||
                            (tek.tip == luchshaya.tip &&
                                memcmp(tek.znacheniya, luchshaya.znacheniya, sizeof(tek.znacheniya)) > 0)) {
                            luchshaya = tek;
                            est = true;
                        }
                    }
                }
            }
        }
    }

    return luchshaya;
}

const char* opisatLuchshuyuRuku(karta ruka[2], karta stol[5]) {
    karta vse[7];
    int n = 0;
    for (int i = 0; i < 2; i++) {
        if (ruka[i].znach != 0) vse[n++] = ruka[i];
    }
    for (int i = 0; i < 5; i++) {
        if (stol[i].znach != 0) vse[n++] = stol[i];
    }

    static char buf[100];

    if (n < 5) {
        int maxCard = 2;
        for (int i = 0; i < n; i++) {
            if (vse[i].znach > maxCard) maxCard = vse[i].znach;
        }
        sprintf_s(buf, sizeof(buf), "Старшая карта (%s)", znachToStr(maxCard));
        return buf;
    }

    RezultatKombinacii luchshaya = naitiLuchshuyuKombinaciyu(ruka, stol);

    const char* imenaTipov[] = {
        "Старшая карта", "Пара", "Две пары", "Тройка", "Стрит",
        "Флеш", "Фулл-хаус", "Каре", "Стрит-флеш", "Роял-флеш"
    };

    if (luchshaya.tip == 0) {
        sprintf_s(buf, sizeof(buf), "%s (%s)", imenaTipov[luchshaya.tip], znachToStr(luchshaya.znacheniya[0]));
    }
    else if (luchshaya.tip == 1) {
        sprintf_s(buf, sizeof(buf), "%s (%s)", imenaTipov[luchshaya.tip], znachToStr(luchshaya.znacheniya[0]));
    }
    else if (luchshaya.tip == 2) {
        sprintf_s(buf, sizeof(buf), "%s (%s и %s)", imenaTipov[luchshaya.tip],
            znachToStr(luchshaya.znacheniya[0]), znachToStr(luchshaya.znacheniya[1]));
    }
    else if (luchshaya.tip == 3) {
        sprintf_s(buf, sizeof(buf), "%s (%s)", imenaTipov[luchshaya.tip], znachToStr(luchshaya.znacheniya[0]));
    }
    else if (luchshaya.tip == 4 || luchshaya.tip == 8) {
        int doZnach = luchshaya.znacheniya[0];
        sprintf_s(buf, sizeof(buf), "%s до %s", imenaTipov[luchshaya.tip], znachToStr(doZnach));
    }
    else if (luchshaya.tip == 6) {
        sprintf_s(buf, sizeof(buf), "%s (%s и %s)", imenaTipov[luchshaya.tip],
            znachToStr(luchshaya.znacheniya[0]), znachToStr(luchshaya.znacheniya[1]));
    }
    else if (luchshaya.tip == 7) {
        sprintf_s(buf, sizeof(buf), "%s (%s)", imenaTipov[luchshaya.tip], znachToStr(luchshaya.znacheniya[0]));
    }
    else {
        strcpy_s(buf, sizeof(buf), imenaTipov[luchshaya.tip]);
    }

    return buf;
}

bool vseKartiUnikalny(karta rukaIgr[2], karta rukaKaz[2], karta stol[5]) {
    karta vse[9];
    int n = 0;
    for (int i = 0; i < 2; i++) vse[n++] = rukaIgr[i];
    for (int i = 0; i < 2; i++) vse[n++] = rukaKaz[i];
    for (int i = 0; i < 5; i++) {
        if (stol[i].znach != 0) {
            vse[n++] = stol[i];
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (vse[i].mast == vse[j].mast && vse[i].znach == vse[j].znach) {
                return false;
            }
        }
    }
    return true;
}

int sravnitRuki(karta rukaIgr[2], karta rukaKaz[2], karta stol[5]) {
    if (!vseKartiUnikalny(rukaIgr, rukaKaz, stol)) {
        cout << "\n\tОШИБКА: одинаковые карты!\n";
        return 0;
    }

    RezultatKombinacii igrok = naitiLuchshuyuKombinaciyu(rukaIgr, stol);
    RezultatKombinacii komp = naitiLuchshuyuKombinaciyu(rukaKaz, stol);

    cout << "\n\tВы играете: " << opisatLuchshuyuRuku(rukaIgr, stol) << "\n";
    cout << "\tКомпьютер играет: " << opisatLuchshuyuRuku(rukaKaz, stol) << "\n";

    if (igrok.tip > komp.tip) {
        cout << "\n\tВы выиграли! Ваша комбинация сильнее.\n";
        return 1;
    }
    else if (igrok.tip < komp.tip) {
        cout << "\n\tКомпьютер выиграл! Его комбинация сильнее.\n";
        return -1;
    }
    else {
        int cmp = memcmp(igrok.znacheniya, komp.znacheniya, sizeof(igrok.znacheniya));
        if (cmp > 0) {
            cout << "\n\tВы выиграли! У вас лучше кикер.\n";
            return 1;
        }
        else if (cmp < 0) {
            cout << "\n\tКомпьютер выиграл! У него лучше кикер.\n";
            return -1;
        }
        else {
            cout << "\n\tПолная ничья! Банк разделён поровну.\n";
            return 0;
        }
    }
}