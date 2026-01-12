// profile.h
#pragma once

// Получить список всех зарегистрированных имён игроков
int poluchitSpisokImyon(char spisok[][50], int maxProfilей);

// Создать новый профиль (запросит имя у пользователя)
const char* sozdatNovyiProfil();

// Выбрать существующий профиль из списка
const char* vybratSushchestvuyushchiyProfil();

// Получить полное имя файла сохранения для игрока
void poluchitImyaFilaSohraneniya(const char* imyaIgroka, char rezultat[100]);

// Получить полное имя файла статистики для игрока
void poluchitImyaFilaStatistiki(const char* imyaIgroka, char rezultat[100]);
