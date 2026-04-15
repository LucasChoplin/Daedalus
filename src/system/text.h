#ifndef text_h //evite les inclusions multiples
#define text_h
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../structs.h"
/** \file text.h
    \brief librairie pour l'affichage de text 
    \author Myriam Laaqira
    \version 1.0
    \date 11 mars 2026
*/

/** \brief initialise les polices de caractères
    \return 0 si tout s'est bien passe, -1 sinon
*/
int initText(void);

/** \brief nettoie les fonts
*/
void cleanupText(void);

/** \brief retourne le font par defaut
    \return pointeur vers le font par defaut
*/
TTF_Font* getDefaultFont(void);
/** \brief retourne le font pour les titres
    \return pointeur vers le font pour les titres
*/
TTF_Font* getTitleFont(void);
/** \brief retourne le font pour le combat
    \return pointeur vers le font pour le combat
*/
TTF_Font* getCombatFont(void);
/** \brief retourne le font pour l'écran de fin
    \return pointeur vers le font pour l'écran de fin
*/
TTF_Font* getEndScreenFont(void);

/** \brief retourne le font pour les noms des classes
    \return pointeur vers le font pour les noms des classes
*/
TTF_Font* getClassNameFont(void);

/** \brief retourne le font pour les stats des classes
    \return pointeur vers le font pour les stats des classes
*/
TTF_Font* getClassStatsFont(void);

/** \brief retourne le font pour l'inventaire
    \return pointeur vers le font pour l'inventaire
*/
TTF_Font* getMenuFont(void);

/** \brief retourne le font pour l'affichage de l'xp
    \return pointeur vers le font pour l'affichage de l'xp
*/
TTF_Font* getXpFont(void);

/** \brief dessine du texte
    \param renderer pointeur vers le renderer
    \param font pointeur vers le font a utiliser
    \param texte le texte
    \param couleur couleur du texte
    \param x coordonnee x d'affichage du texte
    \param y coordonnee y d'affichage du texte
*/
void drawText(SDL_Renderer* renderer, TTF_Font* font, const char* texte, SDL_Color couleur, int x, int y);

/** \brief dessine un bouton avec texte centre
    \param renderer pointeur vers le renderer
    \param bouton pointeur vers la structure du bouton a dessiner
*/
void drawButton(SDL_Renderer* renderer, Bouton* bouton);
/** \brief fonction qui écrit des chiffres
    \param r pointeur vers le moteur de rendu 
    \param nb nombre à afficher 
    \param x paramètre x des coordoonées où afficher le nombre 
    \param y paramètre y des coordoonées où afficher le nombre 
*/
void drawChiffre(SDL_Renderer * r,int nb, int x, int y);
#endif