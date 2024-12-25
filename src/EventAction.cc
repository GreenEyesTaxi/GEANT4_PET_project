#define Pi 3.1415926535  // Определение числа Пи

#include "EventAction.hh"              // Подключение заголовочного файла для класса EventAction
#include "RunAction.hh"                // Подключение заголовочного файла для класса RunAction
#include "PrimaryGeneratorAction.hh"   // Подключение заголовочного файла для класса PrimaryGeneratorAction
#include "DetectorConstruction.hh"      // Подключение заголовочного файла для класса DetectorConstruction

#include "G4Event.hh"                   // Подключение заголовочного файла для класса G4Event
#include "G4RunManager.hh"              // Подключение заголовочного файла для менеджера прогона G4RunManager

#include "G4AnalysisManager.hh"         // Подключение заголовочного файла для менеджера анализа G4AnalysisManager

#include "G4UnitsTable.hh"              // Подключение заголовочного файла для таблицы единиц G4UnitsTable
#include "G4SystemOfUnits.hh"           // Подключение заголовочного файла для системы единиц G4SystemOfUnits

#include <iostream>                     // Подключение библиотеки для ввода-вывода
#include <fstream>                      // Подключение библиотеки для работы с файлами

using namespace std;                   // Использование стандартного пространства имен

const G4int nOfVolumes = 64;          // Константа для количества объемов (детекторов)
const G4double rOfCircle = 63.75 * mm; // Константа для радиуса окружности, на которой расположены детекторы

namespace B1  // Определение пространства имен B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Конструктор класса EventAction
EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)  // Инициализация указателя на объект RunAction
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод, вызываемый в начале каждого события
void EventAction::BeginOfEventAction(const G4Event*)
{
  // Обнуление энергии депонированной в каждом объеме
  for(G4int i = 0; i < nOfVolumes; i++){
    fEdep[i] = 0.;  // Инициализация энергии для каждого объема
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод, вызываемый в конце каждого события
void EventAction::EndOfEventAction(const G4Event*)
{
  // Сбор статистики в RunAction
  // fRunAction->AddEdep(fEdep); // (Закомментировано) Добавление энергии в RunAction

  auto analysisManager = G4AnalysisManager::Instance(); // Получение экземпляра менеджера анализа
  
  //
  // Вычисление координат синограммы
  //
  G4int n1 = -1, n2 = 0; // Инициализация индексов для объемов

  // Поиск объемов, в которых была энергия
  for (G4int i = 0; i < nOfVolumes; i++){
    if (fEdep[i] > 500.0 * keV){ // Проверка, была ли энергия больше 500 кэВ
      if (n1 > 0)
        n2 = i; // Если n1 уже установлен, устанавливаем n2
      else
        n1 = i; // Устанавливаем n1
    }
  }
  
  // Вычисление угла и радиуса для синограммы
  G4double theta = Pi / nOfVolumes * (n1 + n2); // Вычисление угла
  G4double rForSynogram = rOfCircle * cos(theta - 2 * Pi * n1 / nOfVolumes); // Вычисление радиуса
  theta = theta * 180.0 / Pi - 90.0; // Преобразование угла в градусы и коррекция

  //
  // Формирование файла
  //

  fstream ofile; // Объект для работы с файлом
  if (n1 * n2) { // Проверка, были ли найдены два объема    
    std::string clipboard = to_string(rForSynogram) + "\t" + to_string(theta); // Формирование строки с данными радиуса и угла
    ofile.open("raw_synogram.txt", ios::app); // Открытие файла для добавления данных

    if (ofile.is_open()) { // Проверка, успешно ли открыт файл
      //ofile << "Hello world!" << endl; // (Закомментировано) Пример записи в файл
      //ofile << n1 << "; " << n2 << endl; // (Закомментировано) Пример записи индексов
      ofile << clipboard << std::endl; // Запись данных в файл
    }

    ofile.close(); // Закрытие файла
  }

  // Заполнение гистограмм
  analysisManager->FillH1(0, fEdep[0]); // Заполнение первой гистограммы значением энергии для первого объема
  analysisManager->FillH1(1, fEdep[25]); // Заполнение второй гистограммы значением энергии для 26-го объема

  // Заполнение н-тула
  analysisManager->FillNtupleDColumn(0, fEdep[0]); // Заполнение первого столбца н-тула значением энергии для первого объема
  analysisManager->FillNtupleDColumn(1, fEdep[25]); // Заполнение второго столбца н-тула значением энергии для 26-го объема
  analysisManager->AddNtupleRow(); // Добавление строки в н-тула
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} 