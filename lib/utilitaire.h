#ifndef utilitaire_h //evite les inclusions multiples
#define utilitaire_h
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"
#include "def.h"
/** \file utilitaire.h
    \brief bibliothèque des fonctions générals 
    \author Patrick Leguillon
    \version 1.0
    \date 28 février 2026
*/

/** \brief fonction pour vérifier si un fichier existe 
    \param nom chaîne de caractère qui contient le nom du fichier à vérifier
    \return renvoie 1 si le fichier existe et 0 dans le cas contraire 
*/
int fichierExiste(char nom[]);

/** \brief fonction pour charger les images danns une texture (enlève la couleur blanche de l'image aussi (255.255.255))
    \param nom nom du fichier bpm à charger 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers le pointeur de Texture où stockée la Texture
 */
int chargerImage(char nom[],SDL_Renderer * r,SDL_Texture ** t);

/** \brief indique si un clic de souris touche une texture 
    \param e pointeur sur l'event de la souris enclenché sert à récupérer le x et y de la souris 
    \param r pointeur sur le RECT de la texture pour récupérer son x y w et h
    \return 1 si la souris touche la texture et 0 si elle ne touche pas 
*/
int detecterButtonClique(SDL_Event * e,SDL_Rect * r);

/** \brief fonction qui détecte si la souris est dans un rectangle 
    \param x coordonnée x de la souris
    \param y coordonnée y de la souris
    \param r pointeur sur le RECT à tester
    \return 1 si la souris est dans le rectangle et 0 sinon
*/
int detecterSourisDansRect(int x, int y, SDL_Rect * r);

/** \brief fonction qui affiche un chiffre 
    \param r pointeur vers le moteur de rendu 
    \param t pointeur vers la texture de chiffre de 32px * 32 px
    \param nb chiffre à afficher compris entre 0 et 9 
    \param d pointeur vers le Rect contenant le x et y ou afficher le chiffre 
*/
void afficherChiffre(SDL_Renderer * r,SDL_Texture * t,int nb,SDL_Rect * d);

/** \brief fonction pour sauvegarder les données du jeu
    \param fighter pointeur vers le perso 
    \param listeItem pointeur vers la liste des items
    \param etageActuel entier qui indique l'étage actuel du donjon 
*/
void saveGameData(Fighter *fighter, item_t *listeItem[], int etageActuel);

/** \brief fonction pour charger les données du jeu 
    \param fighter pointeur vers le perso où seront stockés les données de personnage chargées
    \param listeItem pointeur vers la liste des items où seront stockés les données des items chargées
    \param etageActuel pointeur vers un entier où sera stocké l'étage chargé 
*/
void chargerDonnées(Fighter *fighter, item_t *listeItem[], int * etageActuel);

/** \brief fonction qui réinitialise des donnés du jeu en cas de défaite du joueur
    \param p pointeur vers le perso du joueur pour charger l'ancienne sauvegarde 
    \param l pointeur vers la liste des items pour charger l'ancienne sauvegarde
    \param etageActuel pointeur vers la variable etageActuel pour charger l'ancienne sauvegarde
    \param coffre pointeur vers la variable coffre pour la réinitialiser à 0
     \param stockMarchand pointeur vers le struct loot_t où sera stocké le nouveau stock du marchand
*/
void defaite(Fighter * p, item_t * l[], int * etageActuel, int * coffre, loot_t * stockMarchand);

#endif 