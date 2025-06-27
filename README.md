# 📒 MineContact

MineContact est une application console développée en langage **C** qui permet de **gérer un carnet d’adresses** : ajouter, supprimer, modifier et rechercher des contacts.

---

## 🚀 Fonctionnalités principales

✅ Ajouter un nouveau contact  
✅ Supprimer un contact existant  
✅ Rechercher un contact par nom  
✅ Afficher tous les contacts enregistrés  
✅ Sauvegarder les données dans un fichier texte  
✅ Charger automatiquement les contacts au démarrage

---

## 💡 Technologies utilisées

- Langage : **C**
- Compilation : `gcc`
- Fichiers : Manipulation via `fopen`, `fprintf`, `fscanf`...
- Stockage : Fichier texte `.txt`

---

## 🖼️ Captures d’écran

### Menu principal
![Menu principal](Capture/Capture%20d'%C3%A9cran%202025-06-21%20000233.png)

---

### Ajout d’un contact
![Ajout contact](Capture/Capture%20d'%C3%A9cran%202025-06-21%20000303.png)

---

### Liste des contacts
![Liste contacts](Capture/Capture%20d'%C3%A9cran%202025-06-21%20000547.png)

---

## 📁 Structure du projet

```bash
MineContact/
├── contact.c               # Fonctions de gestion des contacts
├── contact.h               # Fichier d'en-tête
├── main.c                  # Fonction principale avec menu
├── fichier.txt             # Base de données locale (si présente)
├── Capture/                # Dossier contenant les captures d’écran
└── README.md               # Ce fichier de présentation

### ⚙️ Compilation

```bash
gcc main.c contact.c -o MineContact



---

## 👨‍💻 Auteur

**Maurice Louba**  
📫 GitHub : [@Maurice-Louba](https://github.com/Maurice-Louba)

---

## 📜 Licence

Ce projet est open source et peut être utilisé **librement à des fins éducatives**.  
Tu peux le modifier, l'utiliser et le partager librement.

---
