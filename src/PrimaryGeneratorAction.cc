#define Pi 3.1415926535  // Определение числа Пи

#include <time.h>                     // Подключение библиотеки для работы с временем
#include <math.h>                     // Подключение библиотеки для математических функций
#include <random>                     // Подключение библиотеки для генерации случайных чисел

#include "PrimaryGeneratorAction.hh"  // Подключение заголовочного файла для класса PrimaryGeneratorAction

#include "G4LogicalVolumeStore.hh"    // Подключение заголовочного файла для хранения логических объемов
#include "G4LogicalVolume.hh"         // Подключение заголовочного файла для логического объема
#include "G4Box.hh"                   // Подключение заголовочного файла для создания кубов
#include "G4RunManager.hh"            // Подключение заголовочного файла для менеджера прогона
#include "G4ParticleGun.hh"           // Подключение заголовочного файла для пушки частиц
#include "G4ParticleTable.hh"         // Подключение заголовочного файла для таблицы частиц
#include "G4ParticleDefinition.hh"    // Подключение заголовочного файла для определения частиц
#include "G4SystemOfUnits.hh"         // Подключение заголовочного файла для системы единиц
#include "Randomize.hh"               // Подключение заголовочного файла для генерации случайных чисел
#include "G4GenericMessenger.hh"      // Подключение заголовочного файла для работы с сообщениями
#include "G4RandomDirection.hh"       // Подключение заголовочного файла для генерации случайных направлений

const G4double innerRadius = 20 * mm; // Внутренний радиус
const G4double randRMin = 0. * mm;    // Минимальное значение радиуса для случайной генерации
const G4double randRMax = 5. * mm;    // Максимальное значение радиуса для случайной генерации
const G4double randAMin = 0. * mm;     // Минимальное значение угла для случайной генерации
const G4double randAMax = 2 * Pi * mm; // Максимальное значение угла для случайной генерации

using namespace std; // Использование стандартного пространства имен

// Функция для генерации случайного радиуса
double randRadius() {
  srand(time(NULL)); // Инициализация генератора случайных чисел с текущим временем
  return (double) rand() / (double) RAND_MAX * (randRMax - randRMin) + randRMin; // Генерация случайного радиуса
}

// Функция для генерации случайного угла
double randAngle() {
  srand(time(NULL)); // Инициализация генератора случайных чисел с текущим временем
  return (double) rand() / (double) RAND_MAX * (randAMax - randAMin) + randAMin; // Генерация случайного угла
}

namespace B1 // Определение пространства имен B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Конструктор класса PrimaryGeneratorAction
PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
{
  // Определение новой пушки частиц
  G4int n_particle = 1; // Количество частиц
  fParticleGun  = new G4ParticleGun(n_particle); // Создание объекта пушки частиц
  fParticleGun1 = new G4ParticleGun(n_particle); // Создание второго объекта пушки частиц

  // Определение временной частицы
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable(); // Получение таблицы частиц
  G4String particleName; // Имя частицы
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma"); // Поиск гамма-частицы

  fParticleGun->SetParticleDefinition(particle); // Установка определения частицы для первой пушки
  fParticleGun1->SetParticleDefinition(particle); // Установка определения частицы для второй пушки

  //
  // Определение кинематики частицы
  //
  
  // Определение позиции генератора
  G4double x0 = 0; // Начальная позиция по X
  G4double y0 = 0; // Начальная позиция по Y
  G4double z0 = 0; // Начальная позиция по Z

  /*G4int sourcePos = 9; 

  G4double angle = 2 * Pi / 10 * sourcePos; // Вычисление угла на основе позиции источника

  G4double x0 = 0; // Начальная позиция по X
  G4double y0 = innerRadius * sin(angle); // Вычисление позиции по Y на основе угла и внутреннего радиуса
  G4double z0 = innerRadius * cos(angle); // Вычисление позиции по Z на основе угла и внутреннего радиуса*/

  /*G4double r = 0, phi = 0;

  r = 5 - 5 * G4UniformRand(); // Генерация случайного радиуса в диапазоне от 0 до 5
  phi = 2 * Pi * G4UniformRand(); // Генерация случайного угла

  G4double x0 = 0; // Начальная позиция по X
  G4double y0 = r * sin(phi) + 20; // Вычисление позиции по Y на основе случайного радиуса и угла
  G4double z0 = r * cos(phi) + 20; // Вычисление позиции по Z на основе случайного радиуса и угла
  cout << "(" << x0 << ", " << y0 << ", " << z0 << ")" << endl; // Вывод позиции в консоль*/

  // Установка свойств частицы
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0)); // Установка позиции для первой пушки
  fParticleGun->SetParticleEnergy(0.511 * MeV); // Установка энергии для первой пушки

  fParticleGun1->SetParticlePosition(G4ThreeVector(x0, y0, z0)); // Установка позиции для второй пушки
  fParticleGun1->SetParticleEnergy(0.511 * MeV); // Установка энергии для второй пушки
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Деструктор класса PrimaryGeneratorAction
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun; // Освобождение памяти, занятой первой пушкой частиц
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Метод для генерации первичных частиц
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Этот метод вызывается в начале каждого события
  //
  // Определение вектора импульса частицы

  //G4Random::setTheSeed(time(NULL)); // Установка начального значения для генератора случайных чисел 

  // Случайная точка в круге

  /*G4double r = 0, phi = 0;

  r = 5 - 5 * G4UniformRand(); // Генерация случайного радиуса в диапазоне от 0 до 5
  phi = 2 * Pi * G4UniformRand(); // Генерация случайного угла

  G4double sPosY = 20, sPosZ = 20; // Задание смещения по Y и Z

  G4double x0 = 0; // Начальная позиция по X
  G4double y0 = r * sin(phi) + sPosY; // Вычисление позиции по Y
  G4double z0 = r * cos(phi) + sPosZ; // Вычисление позиции по Z
  //cout << "(" << x0 << ", " << y0 << ", " << z0 << ")" << endl; // Вывод позиции в консоль*/

  G4double x = 0; // Начальная позиция по X
  G4double angle = 2 * Pi * G4UniformRand(); // Генерация случайного угла
  G4double angle1 = 0; // Временная переменная для второго угла

  // Определение второго угла на основе первого
  if (angle >= Pi)
    angle1 = angle - Pi; // Если угол больше или равен Пи, вычитаем Пи
  else
    angle1 = angle + Pi; // В противном случае добавляем Пи

  // Определение векторов импульса для двух пушек
  G4ThreeVector momentumUnitVector = G4ThreeVector(0., sin(angle), cos(angle)); // Вектор импульса для первой пушки
  G4ThreeVector momentumUnitVector1 = G4ThreeVector(0., sin(angle1), cos(angle1)); // Вектор импульса для второй пушки

  // Установление направления импульса в фиксированном направлении
  //G4ThreeVector momentumUnitVector = G4ThreeVector(0., 0., 1); // Вектор импульса, направленный вдоль оси Z
  //G4ThreeVector momentumUnitVector1 = G4ThreeVector(0., 0., -1); // Вектор импульса, направленный в противоположном направлении вдоль оси Z

  // Установка позиции для первой пушки частиц
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0)); // Установка позиции для первой пушки
  fParticleGun->SetParticleMomentumDirection(momentumUnitVector); // Установка направления импульса для первой пушки
  fParticleGun->GeneratePrimaryVertex(anEvent); // Генерация первичного вершинного события для первой пушки

  // Установка позиции для второй пушки частиц
  //fParticleGun1->SetParticlePosition(G4ThreeVector(x0, y0, z0)); // Установка позиции для второй пушки
  fParticleGun1->SetParticleMomentumDirection(momentumUnitVector1); // Установка направления импульса для второй пушки
  fParticleGun1->GeneratePrimaryVertex(anEvent); // Генерация первичного вершинного события для второй пушки
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

} 