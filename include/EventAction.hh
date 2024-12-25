#ifndef B1EventAction_h // Проверка, определен ли уже заголовочный файл
#define B1EventAction_h 1 // Определение заголовочного файла

#include "G4UserEventAction.hh" // Подключение заголовочного файла для базового класса G4User EventAction
#include "globals.hh" // Подключение заголовочного файла для глобальных определений

/// Event action class
///

namespace B1 // Определение пространства имен B1
{

class RunAction; // Предварительное объявление класса RunAction

class EventAction : public G4UserEventAction // Определение класса EventAction, наследующего от G4User EventAction
{
  public:
    EventAction(RunAction* runAction); // Конструктор, принимающий указатель на объект RunAction
    ~EventAction() override = default; // Деструктор по умолчанию с переопределением

    void BeginOfEventAction(const G4Event* event) override; // Метод, вызываемый в начале каждого события
    void EndOfEventAction(const G4Event* event) override; // Метод, вызываемый в конце каждого события

    void AddEdep(G4double edep, G4int i) { fEdep[i] += edep; } // Метод для добавления энергии в массив fEdep

  private:
    RunAction* fRunAction = nullptr; // Указатель на объект RunAction
    G4String   name = ""; // Имя события (инициализируется пустой строкой)
    G4double   fEdep[64]; // Массив для хранения энергии, депонированной в 64 объемах
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // Конец проверки заголовочного файла


