/////////////////// INCLUSION DES BIBLIOTHEQUES /////////////////////////

#include <Adafruit_SSD1306.h>               // Bibliothèque de l'écran OLED
#include <SPI.h>                            // Bibliothèque pour communiquer avec le potentiomètre digital
#include <SoftwareSerial.h>                 // Bibliothèque pour gérer la communication avec l'appareil bluetooth

/////////////////// DEFINITION DE PARAMETRES //////////////////////////

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)
// Define the MCP41050 OP command bits (only one POT), potentiomètre 50kOhm max
// Note: command byte format xxCCxxPP, CC command, PP pot number (01 if selected) 
#define MCP_NOP 0b00000000
#define MCP_WRITE 0b00010001
#define MCP_SHTDWN 0b00100001
#define rxPin 5 // Broche 11 en tant que RX, raccordé sur TX du HC-05
#define txPin 3 // Broche 10 en tant que RX, raccordé sur TX du HC-05
#define baudrate 9600 // baudRate à 9600, identique à celui du moniteur série pour synchroniser la communication
SoftwareSerial mySerial(rxPin ,txPin); // Définition du software serial

const int ssMCPin = 10; // Define the slave select for the digital pot

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED); // On nomme ecranOLED la bibliothèque Adafruit pour
// gérer par la suite les commandes de l'écran OLED

////////////// DECLARATION DES VARIABLES /////////////////////////

const int Capteur = A0; // On initialise le capteur à l'entrée analogique A0
int ValeurResBit=80; // valeur de la résistance comprise entre 0 et 1023 bits
//uint8_t ValeurResbitAppli;
float ValeurResBitFloat; // Sert uniquement à récupérer la valeur de ResBit en float pour éviter des types avec le calcul de ValeurResTension
float ValeurResTension; // valeur de la résistance comprise entre 0 et 5V
float ValeurRes; // valeur de la résistance du capteur graphite en OhM
float ValeurPotoR2=255; // Le potentiomètre digital divise ses valeurs de résistance entre 0 et 50kOmh en un intervalle de 256 valeurs (sur 256 bits)
float ValeurPotoR2OLED; // valeur de la résistance R2 qu'on utilisera spécifiquement pour afficher la valeur de R2 sur l'écran OLED
float ValeurPotoR2Calculs; // valeur de la résistance R2 qu'on utilise pour les calculs, R2 est en Ohm ici
float R1=100000; //en Ohm
float R3=100000;
float R5=10000;
float R6=1000;
float Vcc=5; //en Volt

/////////////// FONCTION POUR ECRIRE AU POTENTIOMETRE DIGITAL ///////////////////////////////

void SPIWrite(uint8_t cmd, uint8_t data, uint8_t ssPin) // SPI write the command and data to the MCP IC connected to the ssPin, on s'en sert pour communiquer
//avec le potentiomètre digital
{
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0)); //https://www.arduino.cc/en/Reference/SPISettings
  data = constrain (data, 0, 255); //comme cette fonction sert à communiquer avec le potentiomètre digital qui est sur 256 bits, on restreint les valeurs
  //de la donnée entre 0 et 255, un intervalle entier de 256 valeurs, celles que peut prendre le potentiomètre digital
  digitalWrite(ssPin, LOW); // SS pin low to select chip
  
  SPI.transfer(cmd);        // Send command code
  SPI.transfer(data);       // Send associated value
  
  digitalWrite(ssPin, HIGH);// SS pin high to de-select chip
  SPI.endTransaction(); //à l'issu de toutes ces étapes, on termine la communication des données
}

///////////////////////// SETUP DU PROGRAMME ////////////////////////////////

void setup() {
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED)) // On initialise l'écran OLED et on le prépare à communiquer dans le voidLoop
   while(1);   
  ecranOLED.clearDisplay(); // On clear l'écran avant de rentrer dans le corps du code
  pinMode(rxPin,INPUT); // On définit le pin Rx en INPUT
  pinMode(txPin,OUTPUT); // On définit le pin Tx en OUTPUT
  mySerial.begin(baudrate); // On définit le baudrate de la communication avec l'appareil bluetooth à 9600
  pinMode(Capteur,INPUT); // On définit l'entrée analogique A0 en INPUT, c'est celle qui va recevoir les valeurs de la résistance du capteur graphite
  pinMode (ssMCPin, OUTPUT); //select pin output
  digitalWrite(ssMCPin, HIGH); //SPI chip disabled
  SPI.begin();  
  Serial.begin(9600);
  Serial.println(F("[MCP Digital Pot Test]")); //print to serial
  SPIWrite(MCP_WRITE, ValeurPotoR2, ssMCPin);
}

/////////////////////////// CORPS DU CODE //////////////////////////////////////////

void loop() {
    uint8_t ValeurResBitAppli;
    ValeurResBit=analogRead(Capteur);
    //SPIWrite(MCP_WRITE, ValeurPotoR2, ssMCPin); // On envoie la valeur au potentiomètre digital, 50kOhm pour la première mesure, voir 2 lignes après pour les nouvelles valeurs
    //ValeurResBit=analogRead(Capteur); // On lit la valeur sur l'entree analogique A0, celle du capteur donc, on obtient une valeur entre 0 et 1023, c'est celle qu'on enverra à l'appli via le bluetooth, c'est un int codé sur peut de bits donc moins lourd pour l'envoie des données contrairement à celui en float qui sert pour les calculs
    if (ValeurResBit<20){ // Si la valeur est inférieure à 30, celà veut dire qu'on se rapproche de la limite inférieure (0) et il est donc nécessaire de réajuster la valeur du potentiomètre digital en conséquence. On a ValeurResBit faible pour des résistances très élevées
      ValeurPotoR2=3; // On diminue R2 de beaucoup pour amplifier le signal et recevoir une valeur supérieure à 0
      SPIWrite(MCP_WRITE, ValeurPotoR2, ssMCPin); // On envoie la valeur
    }
    else if (ValeurResBit>1000){ // On utilise cette condition pour des résistances de l'ordre de la centaine de MOhm environ
      ValeurPotoR2=255; // On choisit une valeur de résistance intermédiaire
      SPIWrite(MCP_WRITE, ValeurPotoR2, ssMCPin); // On envoie la valeur
    }
    ValeurResBit=analogRead(Capteur);
    ValeurResBitAppli=ValeurResBit/4;
    Serial.println(ValeurResBitAppli); // Test pour vérifier la valeur mesurer à la sortie de A0 en cass de problèmes avec l'appli; ligne à vocation uniquement utilitaire pour le programmeur
    ValeurResBitFloat=analogRead(Capteur); // Valeur à la sortie de A0, float qui servira pour les calculs, valeur en float pour éviter les conflits avec le type de ValeurResTension
    mySerial.write(ValeurResBitAppli); // On envoie à l'appareil bluetooth, qui communiquera avec l'appli, la valeur à la sortie de A0
    ValeurResTension=ValeurResBitFloat*5/1023; // On convertit le signal à la sortie de A0 en tension; l'arduino ne peut recevoir des tensions que jusqu'à 5V max
    ValeurPotoR2Calculs=ValeurPotoR2*50000/255; // On convertit la valeur de R2 (le potentiomètre digital) échelonné de 0 à 255, en Ohm pour les calculs, 255 correspondant à 50 kOhm
    ValeurRes=(R1*((ValeurPotoR2Calculs+R3)/ValeurPotoR2Calculs)*Vcc/ValeurResTension)-R5-R1; // Valeur exacte de la résistance en fonction des autres composants du circuit
    ValeurRes=ValeurRes/1000000; //en MOhm
    ValeurPotoR2OLED=ValeurPotoR2Calculs/1000; // R2 en kOhm
    
    Serial.println(ValeurRes); // On affiche sur le moniteur série la valeur de la résistance, à plus un intérêt pour le programmeur qui veut s'assurer de la justesse de ses résultats
    delay(200);   // On met un délai de 200ms pour éviter que l'écra/l'arduino ne soit surchargés de données et n'arrivent plus à suivre le traitement des informations
                                       
    ecranOLED.clearDisplay(); // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(2); // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setTextColor(SSD1306_WHITE); // On choisit la couleur du texte sur l'écran OLED
    ecranOLED.setCursor(0, 0); // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
    ecranOLED.print(ValeurRes); // On affiche la valeur de la résistance en MOhm sur l'écran OLED
    ecranOLED.print("MOhm"); // Texte affiché sur l'OLED pour préciser que la résistance du capteur est en MOhm
    ecranOLED.setCursor(0, 20); // On déplace le curseur de l'acran plus bas pour écirre une nouvelle ligne de texte
    ecranOLED.print("Potar: "); // On va écrire la valeur de la résistance du potentiomètre...
    ecranOLED.print(ValeurPotoR2OLED); //... On l'écrit...
    ecranOLED.print(" kOhm"); // ... Et on précise que c'est en kOhm
    ecranOLED.display(); // Transfert le buffer à l'écran
    
  }
