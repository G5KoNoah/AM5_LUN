# AM5_LUN

## 🎯 Présentation du projet

AM5_LUN est un projet développé en C/C++ visant à reproduire une scène réaliste.  
Il utilise des shaders GLSL, gkit utilisant OpenGL.
La scène est composée de models 3D avec un rendu réaliste sur le ciel, l'eau et les lumières.
Un terrai est généré procéduralement avec un placement aléatoire d'arbres.


## ▶️ Détails de la scène
> 👉 Terrain généré
![Texte alternatif](assets/mon_image.png)
> 👉 Île composée de Models 3D
![Texte alternatif](assets/mon_image.png)
> 👉 Lumière directionnelle et points de lumière
![Texte alternatif](assets/mon_image.png)
> 👉 Eau et reflet
![Texte alternatif](assets/mon_image.png)
> 👉 Ciel
![Texte alternatif](assets/mon_image.png)



## 🛠️ Installation & Compilation

### 🔧 Windows avec Visual Studio

```bash
git clone https://github.com/G5KoNoah/AM5_LUN.git
cd AM5_LUN
```
Générer le projet avec la commande "premake5.exe vs2022"
Lancer le projet VS dans le fichier build
Executer le projet Scene3D
Lancer l'application dans le fichier bin

### 🔧 Linux

```bash
git clone https://github.com/G5KoNoah/AM5_LUN.git
cd AM5_LUN
premake4 gmake
make Scene3D
cd bin
./Scene3D
```


## 🧱 Architecture

> 📂 src/ — Code source principal (moteur)
  > Scene — Création de la fenêtre et affichage des objets
  > objets/ — Classes permettant d'afficher les objets correspondants avec leur shader
  > Lights/ — Classes gérant la gestion des lumières
  
> 📂 shaders/ — Fichiers GLSL correspondant aux shaders utilisés

> 📂 data/ — Fichiers chargés dans la scène (PNG JPG pour les textures, MTL pour les matériaux, OBJ pour le model 3D)


## ✨ Auteurs

- Leonard Benazeth-Lapierre
- Ugo Hong
- Noah Hilaire
