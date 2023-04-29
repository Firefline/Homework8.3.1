// Homework8.3.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "observer.cpp"

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    FatalError* fatalError = new FatalError;
    Error* error = new Error;
    Warning* warning = new Warning;
    UnknownError* unknownError = new UnknownError;

    fatalError->SetNext(error)->SetNext(warning)->SetNext(unknownError);

    LogMessage a_LogMessage;
    a_LogMessage.id = 1;
    a_LogMessage.message = "Fatal Error";

    fatalError->Handle(a_LogMessage);
}