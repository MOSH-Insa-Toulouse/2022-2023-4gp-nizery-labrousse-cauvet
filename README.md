# *PROJET CAPTEUR - CG-4127-GAF*

## Introduction

Ce projet s'inscrit dans l'UF "Du capteur au banc de test" de 4ème année de Génie Physique de l'INSA Toulouse. Il s'agit d'un projet low-tech de développement d'un capteur de contrainte et d'un système de mesure pour caractériser le capteur. Le projet est basé sur l'étude de capteurs de contrainte en graphite, dont la résistance varie en fonction de la déformation du capteur. Le but est de concevoir et de fabriquer un shield Arduino incluant le capteur graphite couplé à un circuit analogique communiquant via un microcontrôleur à une application Android.

### Cahier des charges

Les objectifs de ce projet sont :

Simuler le comportement du circuit analogique sur le logiciel LTSpice pour amplifier et filtrer le signal délivré par le capteur.
* Réaliser un code Arduino permettant la mesure de résistance et le contrôle des différents éléments du circuit.
* Concevoir un PCB constituant le shield se fixant sur une carte Arduino UNO.
* Souder l'ensemble des composants du circuit et développer une application Android sur le site MIT App Inventor affichant les mesures de résistance via transmission Bluetooth.
* Réaliser une datasheet caractérisant le capteur pour différents types de crayons graphites en mesurant la variation de résistance relative en fonction de la déformation du capteur grâce à un banc de mesure existant fabriqué en impression 3D.

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

`![](Sans titre1.png)`
