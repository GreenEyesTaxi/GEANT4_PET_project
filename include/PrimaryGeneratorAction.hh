#ifndef B1PrimaryGeneratorAction_h // Проверка, определен ли уже заголовочный файл
#define B1PrimaryGeneratorAction_h 1 // Определение заголовочного файла

#include "G4VUserPrimaryGeneratorAction.hh" // Подключение заголовочного файла для базового класса G4VUser  PrimaryGeneratorAction
#include "G4ParticleGun.hh" // Подключение заголовочного файла для класса G4ParticleGun
#include "globals.hh" // Подключение заголовочного файла для глобальных определений

class G4ParticleGun; // Предварительное объявление класса G4ParticleGun
class G4Event; // Предварительное объявление класса G4Event
class G4Box; // Предварительное объявление класса G4Box

/// Класс действия генератора частиц.
/// 
/// Начальные кинематики — гамма-частица с энергией 6 МэВ, равномерно распределенная
/// перед фантомом на 80% размера фантома по осям (X,Y).

namespace B1 // Определение пространства имен B1
{

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction // Определение класса PrimaryGeneratorAction, наследующего от G4VUser  PrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(); // Конструктор класса
    ~PrimaryGeneratorAction() override; // Деструктор класса с переопределением

    // Метод из базового класса
    void GeneratePrimaries(G4Event*) override; // Метод для генерации первичных частиц

    // Методы для доступа к пушкам частиц
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; } // Получение указателя на первую пушку частиц
    const G4ParticleGun* GetParticleGun1() const { return fParticleGun1; } // Получение указателя на вторую пушку частиц

  private: 
    G4ParticleGun *fParticleGun = nullptr; // Указатель на первую пушку частиц
    G4ParticleGun *fParticleGun1 = nullptr; // Указатель на вторую пушку частиц
    G4Box *fEnvelopeBox = nullptr; // Указатель на объем, в котором происходит генерация частиц
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif 
