# Documentation Technique Anerops

## Introduction

Ce document a pour but de vous permettre de comprendre le fonctionnement
du projet *Anerops* et, de ce fait, de le continuer.

Il contient une explication sur le fonctionnement et la logique du projet.

## Elements

### Matériel

- Camera Intel RealSense SR300
- PC Windows 10 64bit, USB3.0, CPU Intel >= 6ème génération, 8GB RAM

### Logiciel

#### SDK

Nous avons utilisé le SDK Intel RealSense version 2016 R3.

- Driver: `intel_rs_dcm_sr300_3.3.27.5718.exe`
- Base SDK: `intel_rs_sdk_mw_core_offline_11.0.27.1384.exe`
- Extension SDK Face: `intel_rs_sdk_mw_face_hybrid_11.0.28.2787.exe`

Chacun de ces fichiers ont tous étés téléchargés depuis la [Dev Zone d'Intel](https://software.intel.com/en-us/realsense-sdk-windows-eol).
Cette version du SDK a toute fois été abandonnée depuis peu, en faveur de
la version cross-plateform, qui n'est pas compatible avec ce projet.

#### Autre

Nous avons utilisé [Unreal Engine 4.15](https://www.unrealengine.com/en-US/what-is-unreal-engine-4) comme moteur de jeu,
[Visual Studio 2017](https://www.visualstudio.com/) pour compiler le code et [Qt Creator](https://www1.qt.io/download/) pour développer.

## Code

Toute la partie du code communicant avec la cameré a été écrite en **C++**.
Voici une explication du fonctionnement des classes:

`ARealSenseActor`, ou *le manager* est la classe qui s'occupe de communiquer
avec la caméra. Elle récupère à chaque frame la position et rotation du visage
ainsi que la position de chacun des landmarks (points clés sur le visage).
Elle applique un polissage (smoothing) sur chacune des positions avant
de les exposer au reste du programme.
Elle s'occupe aussi de récupérer le flux couleur.

La classe `ABackground` est appelée par le manager et copie les données brutes
sur une texture dynamiques, affichée en jeu sur un plan.

La classe `FaceTrackingAlertHandler` est dérivée de la classe `AlertHandler`
de RealSense et s'occupe de récupérer les alertes de la caméra (par exemple
lorsque le visage est perdu) et de les exposer. Le manager les récupère ensuite
à chaque frame et agit en conséquence.

La partie graphique a été codée avec le language d'Unreal Engine: le **Blueprint**.

Le blueprint `BP_BoneControl` gère le masque en tant que composant. Il utilise
les données exposées par le manager pour, à chaque frame, modifier la position
et la rotation du masque, ainsi que la position de chacun des points du visage
(landmarks).
Il s'occupe aussi du recalibrage. Ce prosessus consiste à capturer la différence
(sous forme de vecteur) entre la position des landmarks sur le visage de
l'utilisateur et la position des os par défaut. Cette différence peut ensuite
être ajoutée à chaque position récupérée depuis la caméra afin de correspondre
au masque avec plus de fidélité.
Il s'occupe également du délais avant la recalibration, de l'interface qui y
est associée et du changement de masque.

## Scène

La scène 3D contient une instance de `BP_BoneControl`, une instance de la classe
`ARealSenseActor`, des lumières et un `ABackground`.

La lumière doit reflèter l'environment dans lequel le projet est installé, c'est
pourquoi une photo-sphère a été projetée autour de la scène.

## Conclusion

Ce projet devrait être porté sous la dernière version du SDK afin de profiter
des prochaines fonctionnalitées.

D'autres visages pourraient être ajoutés, une manière plus simple de les ajouter
pourrait être implémentée, une meilleure gestion des alertes pourrait être mise
en place. En effet ces dernières ont du être désactivées durant le projet, car
à chaque fois qu'une tête disparaissait du champs, le masque se retrouvait
caché, peut importe de quelle tête il s'agissait.
 