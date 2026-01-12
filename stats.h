//stats.h
#pragma once


// Загружает статистику из файла
void zagruzitStatistiku();

// Сохраняет статистику в файл
void sohranitStatistiku();

// Обновляет статистику после завершения руки
// pobeda: true — победа игрока, false — поражение
// summa: размер банка (чистый выигрыш/проигрыш)
void obnovitStatistiku(bool pobeda, int summa);

// Выводит статистику в консоль
void pokazatStatistiku();

// Указать, для какого игрока вести статистику
void ustanovitIgrokaDlyaStatistiki(const char* imya);

// Достижения: загружает из save_ИМЯ.txt
void pokazatDostizheniyaIzFila(const char* imyaIgroka);