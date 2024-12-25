#define Pi 3.1415926535  // Определение числа Пи

#include <vector>
#include <string>
#include <math.h>

#include "DetectorConstruction.hh"  // Подключение заголовочного файла для класса DetectorConstruction

#include "G4RunManager.hh"          // Менеджер запуска Geant4
#include "G4NistManager.hh"         // Менеджер материалов NIST
#include "G4Box.hh"                 // Класс для создания кубов
#include "G4Cons.hh"                // Класс для создания конусов
#include "G4Orb.hh"                 // Класс для создания сфер
#include "G4Sphere.hh"              // Класс для создания сфер
#include "G4Trd.hh"                 // Класс для создания трапеций
#include "G4LogicalVolume.hh"       // Логический объем
#include "G4PVPlacement.hh"         // Физическое размещение
#include "G4SystemOfUnits.hh"       // Система единиц Geant4

#include "G4SubtractionSolid.hh"    // Класс для вычитания объемов

#include "G4LogicalBorderSurface.hh" // Класс для создания границ поверхностей
#include "G4LogicalSkinSurface.hh"   // Класс для создания кожных поверхностей
#include "G4OpticalSurface.hh"       // Класс для оптических поверхностей

#include "G4VisAttributes.hh"        // Атрибуты визуализации
#include "G4Colour.hh"               // Цвета
#include "G4UnitsTable.hh"           // Таблица единиц

// Константы для детектора
const G4double rOfCircle = 63.75 * mm;  // Радиус окружности, на которой расположены детекторы
const G4int nOfDetectors = 64;           // Количество детекторов

namespace B1                             // В качестве основы взят пример В1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Инициализация массива для хранения логических объемов детекторов
  for (G4int i = 0; i < nOfDetectors; i++){
    fScoringVolume[i] = nullptr;  // Устанавливаем начальное значение в nullptr
  }

  // Получаем менеджер материалов NIST
  G4NistManager* nist = G4NistManager::Instance();

  // Параметры окружающего объема
  G4double env_sizeXY = 60 * cm;  // Размеры по X и Y
  G4double env_sizeZ = 60 * cm;    // Размер по Z

  // Опция для проверки перекрытия объемов
  G4bool checkOverlaps = true;

  // Определение элементов и материалов
  G4double a;  // Молярная масса
  G4double density; 
  G4double z;  // Среднее число протонов
  G4int natoms;

  // Определение элементов
  G4Element* H = new G4Element("Hydrogen", "H", z=1, a=1.01 * g / mole);  // Водород
  G4Element* C = new G4Element("Carbon", "C", z=6, a=12.01 * g / mole);  // Углерод
  G4Element* O = new G4Element("Oxygen", "O", z=8, a=16.00 * g / mole);   // Кислород

  // Определение элементов для материала GAGG
  G4Element* Gd = new G4Element("Gadolinium", "Gd", z=64, a=157.25 * g / mole); // Гадолиний
  G4Element* Al = new G4Element("Aluminum", "Al", z=13, a=26.98 * g / mole);    // Алюминий
  G4Element* Ga = new G4Element("Gallium", "Ga", z=31, a=69.72 * g / mole);     // Галлий

  // Определение количества атомов в материале GAGG
  G4double nGd = 3, nAl = 2, nGa = 3, nO = 12; 

  // Создание материала GAGG (Gadolinium Aluminum Gallium Garnet)
  G4Material* GAGG = new G4Material("GAGG", density = 6.63 * g / cm3, 4);
  GAGG->AddElement(Gd, natoms = nGd);
  GAGG->AddElement(Al, natoms = nAl);
  GAGG->AddElement(Ga, natoms = nGa);  
  GAGG->AddElement(O, natoms = nO);

  // Определение свойств материала GAGG
  G4double photonEnergy[] = {2.757 * eV, 8.819 * eV};  // Энергия фотонов
  const G4int nEntries = sizeof(photonEnergy) / sizeof(G4double);  // Количество точек

  // Определение элементов для материала LSO
  G4Element* Lu = new G4Element("Lutetium", "Lu", z=71, a=174.96 * g / mole); // Лютений
  G4Element* Si = new G4Element("Silicon", "Si", z=14, a=28.085 * g / mole);  // Кремний

  // Определение количества атомов в материале LSO
  G4double nlLu = 2, nlSi = 1, nlO = 5;

  // Создание материала LSO (Lutetium Oxyorthosilicate)
  G4Material* LSO = new G4Material("LSO", density = 7.4 * g / cm3, 3);
  LSO->AddElement(Lu, natoms = nlLu);
  LSO->AddElement(Si, natoms = nlSi);
  LSO->AddElement(O, natoms = nlO);

  // Определение свойств материала GAGG
  G4double refractiveIndex1[] = {1.9, 1.9};  // Индекс преломления
  G4double absorption[] = {1.3 * cm, 1.3 * cm};  // Длина поглощения
  G4double scintilFast[] = {0.19, 0.28};  // Быстрое свечение
  G4double scintilSlow[] = {0.93, 1.00};   // Медленное свечение

  // Создание таблицы свойств материала GAGG
  G4MaterialPropertiesTable* myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX", photonEnergy, refractiveIndex1, nEntries);
  myMPT1->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);
  myMPT1->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFast, nEntries);
  myMPT1->AddProperty("SCINTILLATIONCOMPONENT2", photonEnergy, scintilSlow, nEntries);
        
  // Установка постоянных свойств
  myMPT1->AddConstProperty("SCINTILLATIONYIELD", 0.0 / MeV); 
  myMPT1->AddConstProperty("RESOLUTIONSCALE", 1.0);
    // Установка постоянных свойств для GAGG
  myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 92 * ns);  // Время жизни быстрого свечения
  myMPT1->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 630 * ns); // Время жизни медленного свечения
  myMPT1->AddConstProperty("SCINTILLATIONYIELD1", 0.18);             // Выход света

  // Применение таблицы свойств к материалу GAGG
  GAGG->SetMaterialPropertiesTable(myMPT1); 

  // Определение свойств материала LSO
  G4double refractiveIndexLu[] = {1.82, 1.82};  // Индекс преломления для LSO
  G4double absorptionLu[] = {1.1 * cm, 1.1 * cm}; // Длина поглощения для LSO
  G4double scintilFastLu[] = {0.19, 0.28};  // Быстрое свечение для LSO
  
  // Создание таблицы свойств материала LSO
  G4MaterialPropertiesTable* myMPT2 = new G4MaterialPropertiesTable();
 
  myMPT2->AddProperty("RINDEX", photonEnergy, refractiveIndexLu, nEntries);
  myMPT2->AddProperty("ABSLENGTH", photonEnergy, absorptionLu, nEntries);
  myMPT2->AddProperty("SCINTILLATIONCOMPONENT1", photonEnergy, scintilFastLu, nEntries);
        
  // Установка постоянных свойств для LSO
  myMPT2->AddConstProperty("SCINTILLATIONYIELD", 0.0 / MeV); 
  myMPT2->AddConstProperty("RESOLUTIONSCALE", 1.0);
  myMPT2->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 40 * ns); // Время жизни быстрого свечения
  myMPT2->AddConstProperty("SCINTILLATIONYIELD1", 0.18);            // Выход света

  // Применение таблицы свойств к материалу LSO
  LSO->SetMaterialPropertiesTable(myMPT2); 
        
  // Описание свойств SiPM (Silicon Photomultiplier)
  G4Material* SiPM_mat = nist->FindOrBuildMaterial("G4_Si"); // Получение материала кремния
              
  G4double refractiveIndexSiPM[] = {1.4626, 1.4626}; // Индекс преломления для SiPM
  G4double absorptionSiPM[] = {100. * nm, 100. * nm}; // Длина поглощения для SiPM
  G4double efficiencySiPM[] = {0.23, 0.23};           // Эффективность SiPM

  // Создание таблицы свойств для SiPM
  G4MaterialPropertiesTable* myMPT3 = new G4MaterialPropertiesTable();
  myMPT3->AddProperty("RINDEX", photonEnergy, refractiveIndexSiPM, nEntries, true);
  myMPT3->AddProperty("ABSLENGTH", photonEnergy, absorptionSiPM, nEntries, true);    
  myMPT3->AddProperty("EFFICIENCY", photonEnergy, efficiencySiPM, nEntries, true);

  // Применение таблицы свойств к материалу SiPM
  SiPM_mat->SetMaterialPropertiesTable(myMPT3);

  // Создание мирового объема
  G4double world_sizeXY = 1.2 * env_sizeXY; // Размеры мира по X и Y
  G4double world_sizeZ  = 1.2 * env_sizeZ;   // Размер мира по Z
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR"); // Материал мира - воздух

  // Создание геометрии мира (куб)
  auto solidWorld = new G4Box("World",                           // Имя
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // Размеры

  // Создание логического объема мира
  auto logicWorld = new G4LogicalVolume(solidWorld,  // Геометрия
    world_mat,                                       // Материал
    "World");                                        // Имя

  // Размещение физического объема мира
  auto physWorld = new G4PVPlacement(nullptr,  // Без вращения
    G4ThreeVector(),                           // Позиция (0,0,0)
    logicWorld,                                // Логический объем
    "World",                                   // Имя физического объема
    nullptr,                                   // Нет родительского объема
    false,                                     // Без булевых операций
    0,                                         // Номер копии
    checkOverlaps);                            // Проверка перекрытия

  // Определение формы кристаллов GAGG
  G4double shapeGAGG_dx = 0.15 * cm;  // Размер по X
  G4double shapeGAGG_dy = 0.15 * cm;  // Размер по Y
  G4double shapeGAGG_dz = 1.0 * cm;    // Размер по Z
  
  std::string shape;  // Строка для имени формы
  G4Box* solidShapeSc[nOfDetectors];  // Массив для хранения геометрии кристаллов
  G4LogicalVolume* logicShapeSc[nOfDetectors]; // Массив для хранения логических объемов кристаллов
  G4VPhysicalVolume* physVolumeSc[nOfDetectors]; // Массив для хранения физических объемов кристаллов

  // Создание кристаллов
  for (int i = 0; i < nOfDetectors; i++){
    shape = "Shape" + std::to_string(i); // Генерация имени для каждого кристалла
    G4Box* solidShape = new G4Box(shape, shapeGAGG_dx, shapeGAGG_dy, shapeGAGG_dz); // Создание геометрии кристалла
    G4LogicalVolume* logicShape = new G4LogicalVolume(solidShape, GAGG, shape); // Создание логического объема кристалла
    double angle = 2 * Pi * i / nOfDetectors; // Вычисление угла для размещения кристаллов по кругу
    double posX = 0; // Позиция по X
    double posY = sin(angle) * rOfCircle; // Позиция по Y
    double posZ = cos(angle) * rOfCircle; // Позиция по Z
    G4RotationMatrix* xRot = new G4RotationMatrix; // Создание матрицы вращения
    xRot->rotateX(angle); // Вращение кристалла
    G4VPhysicalVolume* physVolume = new G4PVPlacement(xRot, G4ThreeVector(posX, posY, posZ), logicShape, shape, logicWorld, false, 0, checkOverlaps); // Размещение кристалла
    solidShapeSc[i] = solidShape; // Сохранение геометрии кристалла
    logicShapeSc[i] = logicShape; // Сохранение логического объема кристалла
    physVolumeSc[i] = physVolume; // Сохранение физического объема кристалла
  }

  // Установка первого кристалла GAGG как объема для подсчета
  fScoringVolume[0] = logicShapeSc[0];

  // Сохранение всех логических объемов кристаллов в массиве fScoringVolume
  for (G4int i = 0; i < nOfDetectors; i++){
    fScoringVolume[i] = logicShapeSc[i];
  }

  // Установка атрибутов визуализации для кристаллов
  G4VisAttributes* CrystallVisAtt = new G4VisAttributes(G4Colour(255.0, 255.0, 0.0)); // Цвет кристаллов (желтый)
  CrystallVisAtt->SetVisibility(true); // Установка видимости
  for (int i = 0; i < nOfDetectors; i++){
    logicShapeSc[i]->SetVisAttributes(CrystallVisAtt); // Применение атрибутов визуализации к каждому кристаллу
  }

  // Всегда возвращаем физический объем мира
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}