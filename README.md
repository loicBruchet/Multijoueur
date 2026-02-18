# - Projet Multijoueur  🎮

> **Statut :** Démo
> 
> **Type :** Battle royale
> 
> **Durée du projet :** 2 semaines

## 📖 Introduction
Ce projet a été conçu dans le but principal d'apprendre et de maîtriser les fondements du développement de jeux vidéo en réseau. Nous avons donc du réaliser un jeu en 2 semaines, un jeu de tir en multijoueur afin de comprendre comment synchroniser un état de jeu entre plusieurs machines en temps réel.

Le mode multijoueur ne marche que en local filaire.

## 🛠️ Stack Technique
* **Moteur :** CPU Engine (moteur personnalisé de notre intervenant)
* **Langage :** C++
* **Outils de versionning :** GitHub

## 🧠 Ce que j'ai appris (Key Learnings)

### 1. Multithreading

Le multithreading m'a appris à diviser les tâches permettant de réaliser 
plusieurs opérations en même temps (ici le lien client serveur et le gameplay).


### 2. L'envoi de packets

Les packets servent à envoyer toutes les informations de ce que l'on réalise en jouant (tirer, avancer...),
Ils sont envoyés au serveur qui va ensuite distribuer à tous les clients pour mettre à jour leur jeu.


## 🚀 Améliorations Futures
Si je devais continuer ce projet, je me concentrerais sur :
* Amélioration du Gameplay de manière générale
* Ajouter des collisions
* Restructuration de la partie serveur
