#include "SteppingAction.hh"            // Подключение заголовочного файла для класса SteppingAction
#include "EventAction.hh"               // Подключение заголовочного файла для класса EventAction
#include "DetectorConstruction.hh"       // Подключение заголовочного файла для класса DetectorConstruction

#include "G4Step.hh"                    // Подключение заголовочного файла для класса G4Step
#include "G4Event.hh"                   // Подключение заголовочного файла для класса G4Event
#include "G4RunManager.hh"              // Подключение заголовочного файла для менеджера прогона
#include "G4LogicalVolume.hh"           // Подключение заголовочного файла для логического объема

const G4int nOfVol = 64;               // Константа для количества объемов (детекторов)

namespace B1 // Определение пространства имен B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Конструктор класса SteppingAction
SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction) // Инициализация указателя на объект EventAction
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод, вызываемый на каждом шаге
void SteppingAction::UserSteppingAction(const G4Step* step)
{
  /*if (!fScoringVolumeS) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUser DetectorConstruction());
    fScoringVolumeS = detConstruction->GetScoringVolume(0);
  }*/

  // Получение логического объема текущего шага
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle() // Получение точки перед шагом
      ->GetVolume()->GetLogicalVolume(); // Получение логического объема

  std::string name = volume->GetName(); // Получение имени логического объема
  // Проверка, находимся ли мы в объеме для оценки
  for (G4int i = 0; i < nOfVol; i++) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction()); // Получение объекта DetectorConstruction
    fScoringVolumeS = detConstruction->GetScoringVolume(i); // Получение i-го объема для оценки
    if (name == fScoringVolumeS->GetName()) { // Если имя совпадает с именем объема для оценки
      G4double edepStep = step->GetTotalEnergyDeposit(); // Получение энергии, депонированной на текущем шаге
      fEventAction->AddEdep(edepStep, i); // Добавление энергии в объект EventAction
    }
  }
  // if (volume != fScoringVolume) return; // Проверка, является ли текущий объем объемом для оценки
  
  // Проверка, в каком логическом объеме мы находимся

  // Сбор энергии, депонированной на этом шаге
  // G4double edepStep = step->GetTotalEnergyDeposit(); // Получение энергии, депонированной на текущем шаге
  // fEventAction->AddEdep(edepStep, 0); // Добавление энергии в объект EventAction
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} 