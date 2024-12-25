#ifndef B1RunAction_h // Проверка, определен ли уже заголовочный файл
#define B1RunAction_h 1 // Определение заголовочного файла

#include "G4UserRunAction.hh" // Подключение заголовочного файла для базового класса G4User RunAction
#include "G4Accumulable.hh" // Подключение заголовочного файла для класса G4Accumulable
#include "globals.hh" // Подключение заголовочного файла для глобальных определений

class G4Run; // Предварительное объявление класса G4Run

/// Класс действия прогона
///
/// В методе EndOfRunAction() вычисляется доза в выбранном объеме
/// на основе энергии, накопленной через действия на шаге и события.
/// Вычисленная доза затем выводится на экран.

namespace B1
{

class RunAction : public G4UserRunAction // Определение класса RunAction, наследующего от G4User RunAction
{
  public:
    RunAction(); // Конструктор класса
    ~RunAction() override = default; // Деструктор по умолчанию с переопределением

    void BeginOfRunAction(const G4Run*) override; // Метод, вызываемый в начале прогона
    void EndOfRunAction(const G4Run*) override; // Метод, вызываемый в конце прогона

    void AddEdep(G4double edep); // Метод для добавления энергии

  private:
    G4Accumulable<G4double> fEdep = 0.; // Накопляемая энергия
    G4Accumulable<G4double> fEdep2 = 0.; // Накопляемая энергия для дисперсии
};

}

#endif 