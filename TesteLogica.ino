// C++ code
//

int cm = 0;
String nivel = "";

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  cm = 50;
  delay(1000);
  Serial.print("Cm: ");
  Serial.println(cm);
  Serial.print("Nivel: ");
  Serial.println(nivel);

  if (cm >= 336) {
    nivel = "[LED Verde] Normal";
  }
  
  if (cm >= 224 && cm < 336) {
    nivel = "[LED Amarelo] Atencao";
  }
  
  if (cm >= 112 && cm < 224) {
    nivel = "[LED Laranja] Alerta";
  }
  
  if (cm < 112) {
    nivel = "[LED Vermelho] Critico";
  }
}
