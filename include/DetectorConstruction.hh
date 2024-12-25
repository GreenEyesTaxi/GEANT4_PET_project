#ifndef B1DetectorConstruction_h // Проверка, определен ли уже заголовочный файл
#define B1DetectorConstruction_h 1 // Определение заголовочного файла

#include "G4VUserDetectorConstruction.hh" // Подключение заголовочного файла для базового класса G4VUser DetectorConstruction
#include "globals.hh" // Подключение заголовочного файла для глобальных определений
#include <vector> // Подключение библиотеки для работы с векторами

class G4VPhysicalVolume; // Предварительное объявление класса G4VPhysicalVolume
class G4LogicalVolume; // Предварительное объявление класса G4LogicalVolume

/// Detector construction class to define materials and geometry.
namespace B1 // Определение пространства имен B1
{

class DetectorConstruction : public G4VUserDetectorConstruction // Определение класса DetectorConstruction, наследующего от G4VUser DetectorConstruction
{
  public:
    DetectorConstruction() = default; // Конструктор по умолчанию
    ~DetectorConstruction() override = default; // Деструктор по умолчанию с переопределением

    G4VPhysicalVolume* Construct() override; // Метод для построения физического объема

    G4LogicalVolume* GetScoringVolume(G4int i) const { return fScoringVolume[i]; } // Метод для получения логического объема для оценки по индексу

  protected:
    G4LogicalVolume* fScoringVolume[64]; // Массив логических объемов для хранения 64 объемов для оценки
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif // Конец проверки заголовочного файла
