void setup()
{
  Serial.begin(9600); // nastaveni ser. portu 
}
void loop()    // nekonecna smycka 
{   
  int reading = analogRead(0);   //do promenne reading se bude ukladat aktualni hodnota z analog. vstupu "0"
  float voltage = reading * 5.0; // prevod do napeti     
  voltage /= 1024.0; // napeti / 1024 --> provod do jedne z 1024 urovni 
  Serial.print("Aktualni napeti na senzoru je: ");
  Serial.print(voltage);
  Serial.println(" V"); //  vypis aktualniho napeti na senzoru  
  float temperature = (voltage - 0.5) * 100 ;     //konverze napeti na stupne Celsia ((voltage - 500mV) * 100)  
  Serial.print("Aktualni teplota je: ");
  Serial.print(temperature); 
  Serial.println(" C"); //  vypis aktualni teploty  
  Serial.println("");   
  delay(5000);  
}
