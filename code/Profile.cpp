#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <windows.h>
#include <limits>
#include "Profile.h"
#include "ConsoleUI.h"
#include "InputValidator.h"
#include "Statistics.h"
#include "Exceptions.h"
const char* PROFILE_LIST_FILE = "poker_profiles.list";//файл списка профилей
void Profile::trimSpaces(char* str) {//обреж пробелы
    int len = strlen(str);
    if (len == 0) return;
    int start = 0;
    while (start < len && str[start] == ' ') start++;
    int end = len - 1;
    while (end >= 0 && str[end] == ' ') end--;
    if (start > end) {
        str[0] = '\0';
        return;
    }
    for (int i = start; i <= end; i++) {
        str[i - start] = str[i];
    }
    str[end - start + 1] = '\0';
}
int Profile::getProfileNames(char names[][50], int maxProfiles) {//получ список имён
    std::ifstream file(PROFILE_LIST_FILE);
    int count = 0;
    if (!file.is_open()) return 0;
    while (count < maxProfiles && file.getline(names[count], 50)) {
        trimSpaces(names[count]);
        if (strlen(names[count]) > 0) count++;
    }
    file.close();
    return count;
}
bool Profile::addNameToList(const char* name) {//доб имя в список
    if (!name || strlen(name) == 0) return false;
    char list[20][50];
    int count = getProfileNames(list, 20);
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], name) == 0) return true;//уже есть
    }
    std::ofstream file(PROFILE_LIST_FILE, std::ios::app);
    if (!file.is_open()) throw FileNotFoundException("Не удалось открыть файл профилей для записи");
    file << name << "\n";
    file.close();
    return true;
}
const char* Profile::createNewProfile() {//созд новый профиль
    static char name[50];
    system("cls");
    std::cout << "\n\t" << Color::orange << "████████████████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::orange << Color::bold << "                                                              █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█         С О З Д А Н И Е   Н О В О Г О   П Р О Ф И Л Я        █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                              █\n";
    std::cout << "\t████████████████████████████████████████████████████████████████" << Color::reset << "\n\n";
    std::cout << "\t" << Color::brightYellow << "Введите ваше имя (до 15 символов):" << Color::reset << "\n\t";
    std::cin.getline(name, 50);
    trimSpaces(name);
    if (strlen(name) == 0) {//если пустое имя - генерим номер
        char list[20][50];
        int count = getProfileNames(list, 20);
        int maxNum = 0;
        for (int i = 0; i < count; i++) {//ищем макс номер среди Игрок N
            if (strncmp(list[i], "Игрок", 5) == 0) {
                int num = 0;
                if (strlen(list[i]) > 6) {//если есть число после пробела
                    num = atoi(list[i] + 6);
                }
                else if (strlen(list[i]) == 5) {//просто "Игрок" без номера
                    num = 0;
                }
                if (num > maxNum) maxNum = num;
            }
        }
        if (maxNum == 0 && count > 0) {//если есть просто "Игрок" без номера
            for (int i = 0; i < count; i++) {
                if (strcmp(list[i], "Игрок") == 0) {
                    maxNum = 1;
                    break;
                }
            }
            if (maxNum == 0) maxNum = 0;
        }
        if (maxNum == 0) {//если "Игрок" ещё нет вообще
            strcpy(name, "Игрок");
        }
        else {//иначе даём следующий номер
            sprintf(name, "Игрок %d", maxNum + 1);
        }
    }
    if (strlen(name) > 30) name[30] = '\0';//огранич длины
    addNameToList(name);
    if (strcmp(name, "Настя") == 0 || strcmp(name, "Морковка") == 0) {//пасхалка для морковки
        std::cout << "\n\t" << Color::orange << "🥕 Привет, Морковка! Удачи на парах!" << Color::reset << "\n";
        std::cout << "\t" << Color::pink << "Пусть билеты будут лёгкими, а кавалеры — щедрыми!" << Color::reset << "\n";
        pause();
    }
    else {
        std::cout << "\n\t" << Color::brightGreen << "Профиль \"" << Color::gold << name << Color::brightGreen << "\" успешно создан!" << Color::reset << "\n";
        pause();
    }
    Statistics::setCurrentPlayer(name);//иниц стат для нового игрока
    Statistics::save();//созд пустой файл статистики
    return name;
}
const char* Profile::selectExistingProfile() {//выбор профиля
    char list[20][50];
    int count = getProfileNames(list, 20);
    if (count == 0) return createNewProfile();
    system("cls");
    std::cout << "\n\t" << Color::magenta << "████████████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::magenta << "                                                          █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█               В Ы Б Е Р И Т Е   И Г Р О К А              █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                          █\n";
    std::cout << "\t████████████████████████████████████████████████████████████" << Color::reset << "\n\n";
    for (int i = 0; i < count; i++) {
        std::cout << "\t  " << Color::brightYellow << (i + 1) << ". " << Color::brightWhite << list[i] << Color::reset << "\n";
    }
    std::cout << "\n\t  " << Color::brightGreen << (count + 1) << ". Создать новый профиль" << Color::reset << "\n";
    std::cout << "\t  " << Color::gray << (count + 2) << ". Выйти" << Color::reset << "\n";
    std::cout << "\n\t" << Color::brightCyan << "Ваш выбор: " << Color::reset;
    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > count + 2) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n\t" << Color::brightRed << "Неверный выбор. " << Color::orange << "Попробуйте снова: " << Color::reset;
    }
    std::cin.ignore();//очист буфера
    if (choice == count + 2) exit(0);//выход
    if (choice == count + 1) return createNewProfile();
    static char selectedName[50];
    strcpy(selectedName, list[choice - 1]);
    return selectedName;
}
void Profile::getSaveFileName(const char* playerName, char result[100]) {//имя файла сохранения
    strcpy(result, "save_");
    strcat(result, playerName);
    strcat(result, ".txt");
}
void Profile::getStatsFileName(const char* playerName, char result[100]) {//имя файла статистики
    strcpy(result, "stats_");
    strcat(result, playerName);
    strcat(result, ".txt");
}
bool Profile::deleteProfile() {//удаление профиля
    char list[20][50];
    int count = getProfileNames(list, 20);
    if (count == 0) {
        system("cls");
        std::cout << "\n\t" << Color::brightRed << "Нет профилей для удаления." << Color::reset << "\n";
        pause();
        return false;
    }

    system("cls");
    std::cout << "\n\t" << Color::brightRed << "████████████████████████████████████████████████████████████\n";
    std::cout << "\t█" << Color::brightRed << "                                                          █\n";
    std::cout << Color::brightWhite << Color::bold << "\t█              У Д А Л Е Н И Е   П Р О Ф И Л Я             █\n";
    std::cout << "\t█" << Color::brightWhite << Color::bold << "                                                          █\n";
    std::cout << "\t████████████████████████████████████████████████████████████" << Color::reset << "\n\n";

    for (int i = 0; i < count; i++) {
        std::cout << "\t  " << Color::brightYellow << (i + 1) << ". " << Color::brightWhite << list[i] << Color::reset << "\n";
    }
    std::cout << "\n\t  " << Color::gray << (count + 1) << ". Отмена" << Color::reset << "\n";
    std::cout << "\n\t" << Color::brightCyan << "Выберите номер профиля для удаления: " << Color::reset;

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > count + 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\n\t" << Color::brightRed << "Неверный выбор. Попробуйте снова: " << Color::reset;
    }
    std::cin.ignore();
    if (choice == count + 1) return false; // отмена
    char nameToDelete[50];
    strcpy(nameToDelete, list[choice - 1]);
    std::cout << "\n\t" << Color::orange << "Вы точно хотите удалить профиль \"" << Color::gold << nameToDelete << Color::orange << "\"?" << Color::reset << "\n";
    std::cout << "\t" << Color::brightRed << "Все сохранения и статистика будут утеряны!" << Color::reset << "\n";
    std::cout << "\t" << Color::gray << "(1 — да, 0 — нет): " << Color::reset;
    int confirm = InputValidator::safeInputNumber(0, 1);
    if (confirm == 0) return false;
    char saveFile[100], statsFile[100];
    getSaveFileName(nameToDelete, saveFile);
    getStatsFileName(nameToDelete, statsFile);
    remove(saveFile); // удал файл сохранения
    remove(statsFile); // удал файл статистики
    std::ifstream inFile(PROFILE_LIST_FILE);
    char tempList[20][50];
    int newCount = 0;
    char line[50];
    while (inFile.getline(line, 50)) {
        trimSpaces(line);
        if (strlen(line) > 0 && strcmp(line, nameToDelete) != 0) {
            strcpy(tempList[newCount++], line);
        }
    }
    inFile.close();

    std::ofstream outFile(PROFILE_LIST_FILE);
    for (int i = 0; i < newCount; i++) {
        outFile << tempList[i] << "\n";
    }
    outFile.close();

    std::cout << "\n\t" << Color::brightGreen << "Профиль \"" << Color::gold << nameToDelete << Color::brightGreen << "\" успешно удалён!" << Color::reset << "\n";
    pause();
    return true;
}
bool Profile::saveFileExists(const char* playerName) {//проверка наличия сохранения
    char fileName[100];
    getSaveFileName(playerName, fileName);
    std::ifstream file(fileName);
    return file.good();
}