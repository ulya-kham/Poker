#pragma once
#include <stdexcept>
#include <string>
class InvalidInputException : public std::runtime_error {//искл некорректного ввода
public:
    InvalidInputException(const std::string& msg = "Некорректный ввод") : std::runtime_error(msg) {}//констр
};
class EmptyDeckException : public std::runtime_error {//искл пустой колоды
public:
    EmptyDeckException(const std::string& msg = "Колода пуста") : std::runtime_error(msg) {}//конструкт
};
class FileNotFoundException : public std::runtime_error {//искл файл не найден
public:
    FileNotFoundException(const std::string& msg = "Файл не найден") : std::runtime_error(msg) {}//конструкт
};
class InsufficientChipsException : public std::runtime_error {//искл нехватки фишек
public:
    InsufficientChipsException(const std::string& msg = "Недостаточно фишек") : std::runtime_error(msg) {}//констр
};