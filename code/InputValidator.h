#pragma once
class InputValidator {//валидатор ввода
public:
    static int safeInputNumber(int min, int max);//безопасный ввод числа
    static int makeBet(int chips, int minBet);//ввод ставки (-1 = пас)
};