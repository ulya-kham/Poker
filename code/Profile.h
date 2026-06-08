#pragma once
class Profile {//управление профилями игроков
public:
    static int getProfileNames(char names[][50], int maxProfiles);//получ список имён
    static const char* createNewProfile();//созд новый профиль
    static const char* selectExistingProfile();//выбор сущ профиля
    static void getSaveFileName(const char* playerName, char result[100]);//имя файла сохранения
    static void getStatsFileName(const char* playerName, char result[100]);//имя файла статистики
    static bool saveFileExists(const char* playerName);//проверка наличия сохранения
    static bool deleteProfile();//удаление профиля
private:
    static void trimSpaces(char* str);//обреж пробелы
    static bool addNameToList(const char* name);//доб имя в список
};