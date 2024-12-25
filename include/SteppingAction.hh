#ifndef B1SteppingAction_h // Проверка, определен ли уже заголовочный файл
#define B1SteppingAction_h 1 // Определение заголовочного файла

#include "G4UserSteppingAction.hh" // Подключение заголовочного файла для базового класса G4User SteppingAction
#include "globals.hh" // Подключение заголовочного файла для глобальных определений

class G4LogicalVolume; // Предварительное объявление класса G4LogicalVolume

/// Класс действия на шаге
///

namespace B1
{

class EventAction; // Предварительное объявление класса EventAction

class SteppingAction : public G4UserSteppingAction // Определение класса SteppingAction, наследующего от G4User SteppingAction
{
  public:
    SteppingAction(EventAction* eventAction); // Конструктор класса, принимающий указатель на объект EventAction
    ~SteppingAction() override = default; // Деструктор по умолчанию с переопределением

    // Метод из базового класса
    void UserSteppingAction(const G4Step*) override; // Метод, вызываемый на каждом шаге

  private:
    EventAction* fEventAction = nullptr; // Указатель на объект EventAction
    G4LogicalVolume* fScoringVolumeS = nullptr; // Указатель на логический объем для оценки
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 