# AM5_LUN

## 🎯 Présentation du projet

AM5_LUN est un projet développé en C/C++ visant à reproduire une scène réaliste.  
Il utilise des shaders GLSL, gkit utilisant OpenGL.
La scène est composée de models 3D avec un rendu réaliste sur le ciel, l'eau et les lumières.
Un terrai est généré procéduralement avec un placement aléatoire d'arbres.


## ▶️ Détails de la scène
> 👉 **Terrain généré**  
> ![](readme/mountain.png)  
>
> 👉 **Île composée de models 3D**  
> ![](readme/island.png)  
>
> 👉 **Lumière directionnelle et points de lumière**  
> ![](readme/light.png)  
>
> 👉 **Eau et reflets**  
> ![](readme/water.png)  
>
> 👉 **Ciel dynamique**  
> ![](readme/sky.png)



## 🛠️ Installation & Compilation

### 🔧 Windows avec Visual Studio

> 1️⃣ **Cloner le projet**  
> ```bash
> git clone https://github.com/G5KoNoah/AM5_LUN.git
> cd AM5_LUN
> ```  
>
> 2️⃣ **Générer les fichiers Visual Studio**  
> Exécuter : `premake5.exe vs2022`  
>
> 3️⃣ **Ouvrir Visual Studio**  
> Ouvrir le projet généré dans `build/`  
>
> 4️⃣ **Compiler**  
> Compiler le projet **Scene3D**  
>
> 5️⃣ **Exécuter**  
> Lancer l’exécutable dans `bin/`


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

> 📂 **src/** — Code source principal  
> &emsp;• **Scene/** — Création de la fenêtre et affichage des objets  
> &emsp;• **objets/** — Classes permettant d'afficher les objets avec leur shader  
> &emsp;• **Lights/** — Gestion des différentes lumières  

> 📂 **shaders/** — Fichiers GLSL utilisés dans le rendu  

> 📂 **data/** — Fichiers chargés dans la scène  
> &emsp;• Textures (.png, .jpg)  
> &emsp;• Matériaux (.mtl)  
> &emsp;• Models 3D (.obj)


## ✨ Auteurs

- Leonard Benazeth-Lapierre
- Ugo Hong
- Noah Hilaire
