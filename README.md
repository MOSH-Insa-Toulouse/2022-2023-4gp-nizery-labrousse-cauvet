# *PROJET CAPTEUR - CG-4127-GAF*

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Autres%20images/Capteur_Shield.PNG)

## Introduction : Description du projet, Cahier des charges et Livrables 

Ce projet s’inscrit dans l’UF “Du capteur au banc de test” de 4ème année de Génie Physique de l’INSA Toulouse. Il s’inscrit dans une démarche low-tech de développement d’un capteur de contrainte et d’un système de mesure permettant la caractérisation du capteur. Il s’appuie sur l’étude de capteurs de contrainte en graphite (système granulaire) dont la résistance varie suivant la déformation du capteur (Pencil Drawn Strain Gauges and Chemiresistors on Paper Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang Department of Materials Science and Engineering, Northwestern University 2220 Campus Drive, Evanston, IL, 60208, USA).  
Ce travail comprend le design et la fabrication d’un shield Arduino incluant le capteur graphite couplé à un circuit analogique communiquant via un microcontrôleur à une application Android.
Dans un premier temps, nous avons simulé le comportement du circuit analogique sur le logiciel LTSpice. Ce circuit a le double rôle d’amplifier (via un amplificateur transimpédance) et de filtrer le signal délivré par le capteur.  
Ensuite, nous avons réalisé un code Arduino permettant la mesure de résistance et le contrôle des différents éléments du circuit (écran OLED, module Bluetooth et potentiomètre digital). Nous avons préalablement testé notre circuit et le code en montant les différents éléments sur une breadboard, puis nous avons designé un PCB grâce au logiciel KiCad constituant le shield se fixant sur une carte Arduino UNO.  
Après tirage du PCB, nous avons soudé l’ensemble des composants du circuit et développé une application Android sur le site MIT App Inventor affichant les mesures de résistance via transmission Bluetooth.  
Enfin, nous avons réalisé une datasheet caractérisant le capteur pour différents types de crayons graphites en mesurant la variation de résistance relative en fonction de la déformation du capteur grâce à un banc de mesure existant fabriqué en impression 3D.

### Livrables

Les livrables pour ce projet sont :

* Un shield PCB se fixant à une carte Arduino UNO comprenant un amplificateur transimpédance, un module Bluetooth, un écran OLED et un potentiomètre digital.
* Un code Arduino gérant la mesure de résistance du capteur graphite, la communication avec le module Bluetooth, l'affichage à l'écran OLED et la gestion du potentiomètre digital.
* Un Android APK qui gère l'interfaçage du système de mesure sur un téléphone Android.
* La datasheet du capteur de contrainte graphite.

## Matériel nécessaire
* 1 carte Arduino Uno
* 2 Résistance de 100kΩ
* 1 Résistance de 1kΩ
* 1 Potentiomètre digital MCP-41050
* 2 Condensateurs 100nF
* 1 Condensateurs 1µF
* 1 AOP LTC1050
* 1 Écran OLED01
* 1 Module Bluetooth HC-05

## Simulation du circuit transimpédance avec le logiciel LTSpice 

Nous avons réalisé la simulation électrique du circuit associé au shield sur le logiciel LTSpice. Voici le schéma du circuit simulé : 

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Autres%20images/LTSPICE.PNG)

Le capteur de graphite délivre un courant de 100 nA qui représente un signal trop faible pour que le microcontrôleur de la carte Arduino détecte les variations du signal. L’amplificateur transimpédance permet donc de transformer le signal de courant en tension et de l’amplifier. Le circuit est ensuite constitué de divers filtres permettant d’atténuer le bruit lié à l'environnement de la mesure (50Hz, hautes fréquences…).
Plusieurs tests ont été effectués sur le circuit et sur les filtres pour connaître le rôle de chacun d’eux et vérifier sa compatibilité avec la carte Arduino qui gère l’acquisition des données.

## Code arduino

Le code .ino donne les instructions à l’arduino pour traiter les informations reçues par le capteur graphite et pour envoyer les informations au module bluetooth HC-05 qui communique avec l’application APK, au potentiomètre digital ainsi qu’à l’écran OLED. Différentes bibliothèques sont incluses, la bibliothèque Adafruit_SSD1306 permet de faciliter la communication avec l’écran OLED, la bibliothèque SPI nous permet d’écrire au potentiomètre digital ses nouvelles valeurs au besoin, SoftwareSerial gère la communication avec l’appareil bluetooth HC-05. Le code de l’arduino permet donc plusieurs choses : 
* Calculer la résistance du capteur graphite en temps réel
* Modifier la valeur de la résistance du potentiomètre digital, ce qui nous permet de régler la sensibilité de la mesure, ainsi que de capter des signaux dans l’intervalle de bits [0:1023] dans lequel l’arduino est limité.
* Afficher la résistance du capteur graphite et la résistance du potentiomètre digital sur l’écran OLED
* Communiquer à l’application la valeur en bits du signal du capteur graphite en compressant sa valeur sur un byte, qui pourra être analysé par l’application APK.

## Création du PCB sur Kicad 

Cette partie du projet consistait à concevoir le PCB du circuit transimpédance sur le logiciel Kicad. En partant d’un template d’une carte Arduino Uno, il a fallu : 
* Concevoir la schématique du circuit transimpédance en créant des symboles pour les composants non existants dans la base de donnée du logiciel
* Concevoir les empreintes réelles de ces composants non existants en prenant en compte leurs caractéristiques physiques (nombres de pins, distance entre les pins, géométrie des empreintes…)
* Effectuer le routage du circuit à partir du chevelu implémenté grâce à la vue schématique du circuit
* Poser le plan de masse pour référencer les pistes au GND

Vous trouverez ci-dessous l’intégralité des symboles et empreintes créées ainsi que la vue schématique du circuit et le PCB final.

### Symboles et empreintes 

* Module Bluetooth HC-05

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/HC-05_Empreinte.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/HC-05_Schematique.PNG)

* Amplificateur LTC1050

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/LTC1050_Empreinte.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/LTC1050_Schematique.PNG)

* Ecran OLED

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/OLED_Empreinte.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/OLED_Schematique.PNG)

* Potentiomètre digital MCP41050

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/MCP41050_Empreinte.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/MCP41050_Schematique.PNG)

### Schéma Complet

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/Schematique_SHIELD.PNG)

### PCB

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/Empreinte_SHIELD.PNG)

## Fabrication du Shield
 
### Réalisation PCB

Voici ci-dessus le circuit imprimé réalisé à partir d’un calque de PCB. 

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Images%20KiCad/Calque_PCB.PNG)

Le PCB a été réalisé au sein de deux départements de l’INSA Toulouse, le Génie Électrique et Informatique et le Génie Physique. Il a été fabriqué avec une plaquette d’époxy recouverte d’une couche de cuivre d’environ 60𝝻m. Nous avons imprimé sur du calque le circuit ci-dessus, puis nous avons insolé aux UVs notre plaquette avec ce calque pendant une dizaine de minutes. Après avoir retiré la partie de résine non isolée avec un révélateur, nous avons plongé notre plaquette dans un bain de percholorure de fer. Cela a permis de retirer le cuivre qui n’était pas protégé et de révéler les pistes de notre circuit.

### Perçage et soudures 

Une fois le PCB réalisé, il faut percer avec une perceuse électrique les trous pour y mettre les différents composants. Nous avons utilisé deux tailles de pointes pour percer les trous:
un de diamètre 0,8mm pour tous les différents composants et headers des modules OLED et Bluetooth
un de diamètre 1mm pour les broches de connexion de la carte Arduino Uno

Nous avons aussi dû percer deux trous pour y insérer un via pour relier deux points de masse (cela était prévu dans le schéma Kicad).

## Application téléphone APK

Nous avons développé une application Android à l’aide du site MIT App Inventor. Cette application permet de recevoir les données de la carte Arduino par connexion Bluetooth avec le module HC-05 implémenté sur le shield de la carte Arduino. Après connexion Bluetooth, un graphe se dessine permettant de suivre la résistance du capteur graphite en fonction du temps et des contraintes qui lui sont appliquées. La valeur de cette résistance est également affichée implicitement en plus de la valeur de la de résistance du potentiomètre digital.

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Autres%20images/Logo_App.jpg)

## Banc de test  

![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Datasheet/BancDeTest.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Datasheet/TestCompression.PNG)
![](https://github.com/MOSH-Insa-Toulouse/2022-2023-4gp-nizery-labrousse-cauvet/blob/main/Datasheet/TestTension.PNG)

Nous utilisons ce banc de test composé de 7 demi cylindres de différents diamètres allant de 2 à 5 cm avec un pax de 0,5 cm. Nous posons le capteur sur chacun des cylindres et appliquons donc une déformation  sur celui-ci. Cette déformation se retrouve avec la formule ci-dessous : 
$$\epsilon = \frac{e}{2r}$$ 
avec e l’épaisseur du capteur et r le rayon de courbure donc le rayon de chaque demi cylindre.
Nous avons réalisé les mesures en compression et en tension pour chacun de nos capteurs.  

### Résultats obtenus

Vous retrouverez les résultats obtenus avec notre banc de test dans la datasheet [ici](Datasheet/Datasheet_CG-4127-GAF.docx).

### Analyse et pistes d’améliorations

Lors de nos mesures, nous devions reprendre le R0 à chaque essai car celui-ci augmentait entre-temps. Le papier trop fragile, s'abîmait et rendait plus difficile la conduction des électrons, la résistance augmente donc. Il serait donc envisageable de prendre un papier plus résistant qui ne se déformerait pas de manière permanente après chaque mesure.

On peut également penser à l'utilisation d'un dispositif à base d'un servomoteur, en remplacement de notre banc de test, car celui-ci nous permettrait de reproduire les expériences de manière plus fiable, avec tout le temps la même tension/compression exercée entre les différents capteurs.

Afin d'avoir une excellente sensibilité pour toutes les plages de résistance, on aurait pu augmenter le nombre de valeurs de résistance du potentiomètre qui se règlent automatiquement.

## Datasheet 

La datasheet de notre capteur est disponible dans le dossier Datasheet [ici](Datasheet/Datasheet_CG-4127-GAF.docx).

## *Auteurs*

Si vous avez des remarques ou questions complémentaires sur ce projet, n’hésitez pas à nous contacter :

Guillaume Cauvet : cauvet@insa-toulouse.fr  
Fabien Labrousse : labrousse@insa-toulouse.fr  
Audrey Nizery : nizery@insa-toulouse.fr

