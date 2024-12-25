#ifndef B1ActionInitialization_h // Проверка, определен ли уже заголовочный файл
#define B1ActionInitialization_h 1 // Определение заголовочного файла

#include "G4VUserActionInitialization.hh" // Подключение заголовочного файла для базового класса G4VUser ActionInitialization

/// Action initialization class.
namespace B1 // Определение пространства имен B1
{

class ActionInitialization : public G4VUserActionInitialization // Определение класса ActionInitialization, наследующего от G4VUser ActionInitialization
{
  public:
    ActionInitialization() = default; // Конструктор по умолчанию
    ~ActionInitialization() override = default; // Деструктор по умолчанию с переопределением

    void BuildForMaster() const override; // Метод для инициализации действий для мастер-процесса
    void Build() const override; // Метод для инициализации действий для рабочих процессов
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 