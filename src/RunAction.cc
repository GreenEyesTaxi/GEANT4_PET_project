#include "RunAction.hh"                // Подключение заголовочного файла для класса RunAction
#include "PrimaryGeneratorAction.hh"   // Подключение заголовочного файла для класса PrimaryGeneratorAction
#include "DetectorConstruction.hh"      // Подключение заголовочного файла для класса DetectorConstruction
// #include "Run.hh"                    // (Закомментировано) Подключение заголовочного файла для класса Run

#include "G4RunManager.hh"              // Подключение заголовочного файла для менеджера прогона
#include "G4Run.hh"                     // Подключение заголовочного файла для класса Run
#include "G4AccumulableManager.hh"      // Подключение заголовочного файла для менеджера аккумулируемых значений
#include "G4LogicalVolumeStore.hh"      // Подключение заголовочного файла для хранения логических объемов
#include "G4LogicalVolume.hh"           // Подключение заголовочного файла для логического объема
#include "G4UnitsTable.hh"              // Подключение заголовочного файла для таблицы единиц
#include "G4SystemOfUnits.hh"           // Подключение заголовочного файла для системы единиц

#include "G4AnalysisManager.hh"         // Подключение заголовочного файла для менеджера анализа

namespace B1 // Определение пространства имен B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Конструктор класса RunAction
RunAction::RunAction()
{
  // Добавление новых единиц измерения для дозы
  //
  const G4double milligray = 1.e-3 * gray; // Определение миллигрея
  const G4double microgray = 1.e-6 * gray; // Определение микрогрея
  const G4double nanogray  = 1.e-9 * gray; // Определение наногрея
  const G4double picogray  = 1.e-12 * gray; // Определение пикогрея

  // Регистрация новых единиц измерения в системе
  new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
  new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
  new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

  // Регистрация аккумулируемых значений в менеджере аккумулируемых значений
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);  // Регистрация переменной для накопления энергии
    accumulableManager->RegisterAccumulable(fEdep2); // Регистрация второй переменной для накопления энергии

  // Информирование менеджера прогона о том, что не нужно сохранять начальное значение случайного числа
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Создание менеджера анализа
  // Выбор формата вывода осуществляется через указанный
  // файл расширения.
  auto analysisManager = G4AnalysisManager::Instance();

  // Создание директорий для хранения данных
  // analysisManager->SetHistoDirectoryName("histograms"); // (Закомментировано) Установка имени директории для гистограмм
  // analysisManager->SetNtupleDirectoryName("ntuple"); // (Закомментировано) Установка имени директории для н-тула
  analysisManager->SetVerboseLevel(1); // Установка уровня подробности вывода
  analysisManager->SetNtupleMerging(true); // Включение объединения н-тула
    // Примечание: объединение н-тула доступно только с выводом в формате Root

  // Бронирование гистограмм и н-тула
  //

  // Создание гистограмм
  analysisManager->CreateH1("Eabs0", "Edep in absorber 0", 1000, 0., 1000 * keV); // Гистограмма для энергии в первом абсорбере
  analysisManager->CreateH1("Eabs1", "Edep in absorber 1", 1000, 0., 1000 * keV); // Гистограмма для энергии во втором абсорбере

  // Создание н-тула
  //
  analysisManager->CreateNtuple("PET", "Eabs"); // Создание н-тула с именем "PET"
  analysisManager->CreateNtupleDColumn("Eabs0"); // Создание столбца для энергии в первом абсорбере
  analysisManager->CreateNtupleDColumn("Eabs1"); // Создание столбца для энергии во втором абсорбере
  analysisManager->FinishNtuple(); // Завершение настройки н-тула
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод, вызываемый в начале каждого прогона
void RunAction::BeginOfRunAction(const G4Run*)
{
    // Получение менеджера анализа
  auto analysisManager = G4AnalysisManager::Instance();

  // Открытие выходного файла
  //
  G4String fileName = "PET.root"; // Имя выходного файла
  
  analysisManager->OpenFile(fileName); // Открытие файла для записи
  G4cout << "Using " << analysisManager->GetType() << G4endl; // Вывод типа менеджера анализа
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод, вызываемый в конце каждого прогона
void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent(); // Получение количества событий в прогоне
  if (nofEvents == 0) return; // Если событий нет, выход из метода

  // Объединение аккумулируемых значений
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge(); // Объединение значений

  // Вычисление дозы = общая энергия, депонированная в прогоне, и ее дисперсия
  //
  G4double edep  = fEdep.GetValue(); // Получение накопленной энергии
  G4double edep2 = fEdep2.GetValue(); // Получение накопленной энергии для дисперсии
  G4double rms = edep2 - edep * edep / nofEvents; // Вычисление среднеквадратичного отклонения
  if (rms > 0.) rms = std::sqrt(rms); else rms = 0.; // Если rms положительно, вычисляем корень, иначе 0

  const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction()); // Получение объекта DetectorConstruction
  G4double mass = detConstruction->GetScoringVolume(0)->GetMass(); // Получение массы первого объема
  G4double dose = edep / mass; // Вычисление дозы
  G4double rmsDose = rms / mass; // Вычисление среднеквадратичного отклонения дозы

  // Условия прогона
  // Примечание: для "мастера" нет объекта действия генератора первичных частиц
  // в менеджере прогона для многопоточного режима.
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction()); // Получение объекта PrimaryGeneratorAction
  G4String runCondition; // Строка для условий прогона
  if (generatorAction) // Если объект генератора существует
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun(); // Получение пушки частиц
    runCondition += particleGun->GetParticleDefinition()->GetParticleName(); // Добавление имени частицы в условия прогона
    runCondition += " of "; // Добавление текста "of"
    G4double particleEnergy = particleGun->GetParticleEnergy(); // Получение энергии частицы
    runCondition += G4BestUnit(particleEnergy, "Energy"); // Добавление энергии в удобных единицах
  }

  // Печать результатов
  //
  if (IsMaster()) { // Если это мастер-процесс
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------"; // Сообщение о завершении глобального прогона
  }
  else { // Если это локальный процесс
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------"; // Сообщение о завершении локального прогона
  }

  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " " << runCondition // Вывод количества событий и условий прогона
     << G4endl
     << "------------------------------------------------------------"
     << G4endl
     << G4endl;
  
  auto analysisManager = G4AnalysisManager::Instance(); // Получение менеджера анализа
  // Сохранение гистограмм и н-тула
  //
  analysisManager->Write(); // Запись данных в файл
  analysisManager->CloseFile(); // Закрытие выходного файла
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод для добавления энергии в аккумулируемые значения
void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep; // Накопление энергии
  fEdep2 += edep * edep; // Накопление квадрата энергии для вычисления дисперсии
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} 