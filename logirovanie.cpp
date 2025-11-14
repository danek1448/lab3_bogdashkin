#include "logirovanie.h"
#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

void Logirovanie::log(const string& message) {
    string logFileName = "log.txt";
    // открываем файл дл€ добавлени€ записей
    ofstream logFile(logFileName, ios::app);

    // проверка исоздание нового файла 
    if (!logFile.is_open()) {
        ofstream createLogFile(logFileName);
        createLogFile.close();
        logFile.open(logFileName, ios::app);
    }

    if (logFile.is_open()) {
        time_t now = time(0);
        char dt[26]; // буфер дл€ хранени€ строки с датой и временем
        ctime_s(dt, sizeof dt, &now); // читаемый формат времени
        logFile << dt << " - " << message << endl;
        logFile.close();
    }
    else {
        cerr << "Unable to open log file!" << endl;
    }
}