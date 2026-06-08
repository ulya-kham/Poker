#pragma once
class Statistics {//статистика игрока
public:
    static void load();//загруз стат из файла
    static void save();//сохр стат в файл
    static void update(bool won, int amount);//обнов стат после руки
    static void display();//вывод стат в консоль
    static void setCurrentPlayer(const char* name);//уст тек игрока
    static void showAchievementsFromFile(const char* playerName);//показ достижений из файла
private:
    struct Stats {//внутр структура статистики
        int games = 0;//игры
        int wins = 0;//победы
        int losses = 0;//пораж
        long long totalWin = 0;//сумм выигрыш
        long long totalLoss = 0;//сумм проигрыш
    };
    static Stats stats;//статистика
    static char currentPlayerName[50];//имя тек игрока
};