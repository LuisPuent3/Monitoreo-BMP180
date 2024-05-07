#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>

#define BMP180_ADDRESS 0x77

File dataFile;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println("No se ha encontrado el sensor BMP180, asegúrate de haberlo conectado correctamente.");
    while (1);
  }
  
  Serial.print("Iniciando tarjeta SD...");
  if (!SD.begin(10)) {
    Serial.println("¡Error al iniciar la tarjeta SD!");
    return;
  }
  Serial.println("Tarjeta SD lista.");

  dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Presion (Pa),Altura (m)");
    dataFile.close();
  } else {
    Serial.println("Error al abrir datos.txt");
  }
}

void loop() {
  float presion = bmp.readPressure();
  float altura = bmp.readAltitude();

  Serial.print("Presión: ");
  Serial.print(presion);
  Serial.print(" Pa\tAltura: ");
  Serial.print(altura);
  Serial.println(" metros");

  if (SD.begin(10)) {
    dataFile = SD.open("datos.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.print(presion);
      dataFile.print(",");
      dataFile.println(altura);
      dataFile.close();
      delay(100); // Pequeño retraso después de escribir en el archivo
    } else {
      Serial.println("Error al abrir datos.txt");
    }
  } else {
    Serial.println("Tarjeta SD no lista");
  }

  delay(1000); // Espera 1 segundo entre mediciones
}

