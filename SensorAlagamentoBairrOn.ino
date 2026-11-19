// C++ code
//

int cm = 0;
bool BuzzerTrigger = false;
String nivel = "";

long readUltrasonicDistance()
{
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  delay(500);
  digitalWrite(8, HIGH);
  delay(500);
  digitalWrite(8, LOW);
  pinMode(7, INPUT);
  return pulseIn(7, HIGH);
}

void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  cm = 0.01723 * readUltrasonicDistance();
  Serial.print("Cm: ");
  Serial.println(cm);
  Serial.print("Nivel: ");
  Serial.println(nivel);

  if (cm >= 336) {
    nivel = "Normal";
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
  
  if (cm >= 224 && cm < 336) {
    nivel = "Atencao";
    digitalWrite(3, HIGH);
    if (BuzzerTrigger == false) {
      tone(6, 440, 500);
      BuzzerTrigger = true;
    }
  } else {
    digitalWrite(3, LOW);
    BuzzerTrigger = false;
  }
  
  if (cm >= 112 && cm < 224) {
    nivel = "Alerta";
    digitalWrite(4, HIGH);
    tone(6, 440, 150);
    delay(1000);
    tone(6, 440, 150);
  } else {
    digitalWrite(4, LOW);
  }
  
  if (cm < 112) {
    nivel = "Critico";
    digitalWrite(5, HIGH);
    tone(6, 550, 150);
    delay(150);
    tone(6, 650, 150);
  } else {
    digitalWrite(5, LOW);
  }
}