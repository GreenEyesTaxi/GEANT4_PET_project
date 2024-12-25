#include "ActionInitialization.hh"   // Подключение заголовочного файла для класса ActionInitialization
#include "PrimaryGeneratorAction.hh" // Подключение заголовочного файла для класса генератора первичных частиц
#include "RunAction.hh"              // Подключение заголовочного файла для класса действий во время прогона
#include "EventAction.hh"            // Подключение заголовочного файла для класса действий во время события
#include "SteppingAction.hh"         // Подключение заголовочного файла для класса действий на каждом шаге

namespace B1  // Определение пространства имен B1 (за основу брался пример В1, ничего не переименовывала, чтобы не запутаться)
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод для инициализации действий для мастер-процесса
void ActionInitialization::BuildForMaster() const
{
  auto runAction = new RunAction;  // Создание нового объекта RunAction
  SetUserAction(runAction);         // Установка действия для прогона
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод для инициализации действий для рабочих процессов
void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction); // Установка действия генерации первичных частиц

  auto runAction = new RunAction;            // Создание нового объекта RunAction
  SetUserAction(runAction);                   // Установка действия для прогона

  auto eventAction = new EventAction(runAction); // Создание нового объекта EventAction, связанного с RunAction
  SetUserAction(eventAction);                  // Установка действия для событий

  SetUserAction(new SteppingAction(eventAction)); // Установка действия для каждого шага, связанного с EventAction
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}