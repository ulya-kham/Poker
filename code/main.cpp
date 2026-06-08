#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <ctime>
#include <limits>
#include <exception>
#include "ConsoleUI.h"
#include "GameEngine.h"
#include "Profile.h"
#include "Statistics.h"
#include "InputValidator.h"
#include "Exceptions.h"
int main() {//точка входа
    try {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        std::setlocale(LC_ALL, ".UTF8");
        std::srand(static_cast<unsigned int>(std::time(nullptr)));//иниц генератора
        const char* currentPlayer = Profile::selectExistingProfile();//выбор профиля
        Statistics::setCurrentPlayer(currentPlayer);//уст игрока для стат
        while (true) {//глав цикл меню
            system("cls");
            const char* topBorder = "████████████████████████████████████████████████████████████████████";
            const char* emptyLine = "                                                                  ";
            auto utf8_strlen = [](const char* str) -> int {
                int len = 0;
                for (int i = 0; str[i] != '\0'; ) {
                    if ((str[i] & 0xC0) == 0xC0) i += 2;
                    else i += 1;
                    len++;
                }
                return len;
                };
            int nameLen = utf8_strlen(currentPlayer);
            int greetingLen = 18 + nameLen + 1;
            int innerWidth = 64;
            int totalPadding = innerWidth - greetingLen;
            int leftPadding = (totalPadding / 2) + 3;
            int rightPadding = totalPadding - leftPadding;
            std::cout << "\n\t" << Color::blue << topBorder << "\n";
            std::cout << "\t" << Color::blue << "█" << emptyLine << "█\n";
            std::cout << Color::brightWhite << Color::bold << "\t█           П О К Е Р:  Т Е Х А С С К И Й   Х О Л Д Е М            █\n";
            std::cout << "\t" << Color::brightWhite << "█"
                << std::string(leftPadding, ' ')
                << Color::turquoise << Color::bold << "Добро пожаловать, "
                << Color::gold << Color::bold << currentPlayer << Color::reset << Color::brightYellow << "!"
                << Color::brightWhite << std::string(rightPadding, ' ') << "  █\n";
            std::cout << "\t" << Color::brightWhite << "█" << emptyLine << "█\n";
            std::cout << "\t" << Color::brightWhite << topBorder << Color::reset << "\n";
            bool hasSave = Profile::saveFileExists(currentPlayer);//проверка сохранения
            if (hasSave) {
                std::cout << "\n\t  " << Color::brightGreen << "1. Продолжить сохранённую игру" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightYellow << "2. Начать новую игру" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightMagenta << "3. Достижения" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightBlue << "4. Правила игры" << Color::reset << "\n";
                std::cout << "\t  " << Color::gold << "5. Статистика" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightRed << "6. Удалить профиль" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightCyan << "7. Сменить профиль" << Color::reset << "\n";
                std::cout << "\t  " << Color::gray << "8. Выход" << Color::reset << "\n";
                std::cout << "\n\t" << Color::brightCyan << "Ваш выбор: " << Color::reset;
                int choice = InputValidator::safeInputNumber(1, 8);//безопас ввод
                switch (choice) {//обработка выбора
                case 1: GameEngine::startGame(true, currentPlayer); break;//загрузить игру
                case 2: GameEngine::startGame(false, currentPlayer); break;//новая игра
                case 3: Statistics::showAchievementsFromFile(currentPlayer); break;//достижения
                case 4: showRules(); break;//правила
                case 5: Statistics::display(); break;//статистика
                case 6:
                    if (Profile::deleteProfile()) {//если профиль удалён
                        system("cls");
                        std::cout << "\n\t" << Color::brightCyan << "Возврат к выбору профиля..." << Color::reset << "\n";
                        Sleep(1500);
                        currentPlayer = Profile::selectExistingProfile();//выбор нового профиля
                        Statistics::setCurrentPlayer(currentPlayer);//уст игрока для стат
                    }
                    break;
                case 7:
                    system("cls");
                    std::cout << "\n\t" << Color::brightCyan << "Смена профиля..." << Color::reset << "\n";
                    Sleep(1000);
                    currentPlayer = Profile::selectExistingProfile();//выбор нового профиля
                    Statistics::setCurrentPlayer(currentPlayer);//уст игрока для стат
                    break;
                case 8: endGame(); return 0;//выход
                }
            }
            else {
                std::cout << "\n\t  " << Color::brightGreen << "1. Начать новую игру" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightMagenta << "2. Достижения" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightBlue << "3. Правила игры" << Color::reset << "\n";
                std::cout << "\t  " << Color::gold << "4. Статистика" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightRed << "5. Удалить профиль" << Color::reset << "\n";
                std::cout << "\t  " << Color::brightCyan << "6. Сменить профиль" << Color::reset << "\n";
                std::cout << "\t  " << Color::gray << "7. Выход" << Color::reset << "\n";
                std::cout << "\n\t" << Color::brightCyan << "Ваш выбор: " << Color::reset;
                int choice = InputValidator::safeInputNumber(1, 7);//безопас ввод
                switch (choice) {//обработка выбора
                case 1: GameEngine::startGame(false, currentPlayer); break;//новая игра
                case 2: Statistics::showAchievementsFromFile(currentPlayer); break;//достижения
                case 3: showRules(); break;//правила
                case 4: Statistics::display(); break;//статистика
                case 5:
                    if (Profile::deleteProfile()) {//если профиль удалён
                        system("cls");
                        std::cout << "\n\t" << Color::brightCyan << "Возврат к выбору профиля..." << Color::reset << "\n";
                        Sleep(1500);
                        currentPlayer = Profile::selectExistingProfile();//выбор нового профиля
                        Statistics::setCurrentPlayer(currentPlayer);//уст игрока для стат
                    }
                    break;
                case 6:
                    system("cls");
                    std::cout << "\n\t" << Color::brightCyan << "Смена профиля..." << Color::reset << "\n";
                    Sleep(1000);
                    currentPlayer = Profile::selectExistingProfile();//выбор нового профиля
                    Statistics::setCurrentPlayer(currentPlayer);//уст игрока для стат
                    break;
                case 7: endGame(); return 0;//выход
                }
            }
        }
    }
    catch (const InvalidInputException& e) {//обработка искл ввода
        std::cout << "\n\t" << Color::brightRed << "Ошибка ввода: " << e.what() << Color::reset << "\n";
        pause();
    }
    catch (const EmptyDeckException& e) {//обработка искл колоды
        std::cout << "\n\t" << Color::brightRed << "Ошибка колоды: " << e.what() << Color::reset << "\n";
        pause();
    }
    catch (const FileNotFoundException& e) {//обработка искл файла
        std::cout << "\n\t" << Color::brightRed << "Ошибка файла: " << e.what() << Color::reset << "\n";
        pause();
    }
    catch (const InsufficientChipsException& e) {//обработка искл фишек
        std::cout << "\n\t" << Color::brightRed << "Ошибка фишек: " << e.what() << Color::reset << "\n";
        pause();
    }
    catch (const std::exception& e) {//обработка станд искл
        std::cout << "\n\t" << Color::brightRed << "Критическая ошибка: " << e.what() << Color::reset << "\n";
        pause();
    }
    catch (...) {//обработка неизвестных искл
        std::cout << "\n\t" << Color::brightRed << "Неизвестная ошибка! Программа будет завершена." << Color::reset << "\n";
        pause();
    }
    return 0;
}