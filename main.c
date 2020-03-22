#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//1. Le type du dernier paramètre est une fonction de R dans R.

// ============= PARTIE FAITE PAR ASTRID ===================

float integrale_Rectangles(float a, float b, int nb_subdivisions, float f(float)){
    float somme = 0.0f;
    float h = (b-a) / ((float)nb_subdivisions);
    float x = a;
    for(int i=0; i<nb_subdivisions+1; i++){
        somme+=f(x)*h;
        x+=h;
    }
    return somme;
}

float fonction(float x){
    return x;
}

float integrale_Trapezes(float a, float b, int nb_subdivisions, float f(float)){
    float somme = 0.0f;
    float x;
    float y;
    for(int i=0; i<nb_subdivisions; i++){
        x=a+(b-a)*i/nb_subdivisions;
        y=a+(b-a)*(i+1)/nb_subdivisions;

        somme += (f(x) + f(y)) / 2.0f * (y-x);
    }
    return somme;
}

//Fonction exponentielle, f' = f
float g_exponentielle(float x, float y){
    return y;
}

// ===========================================================
// Fin de la partie faite par Astrid

//Fonction carré pour test
float g_carre(float x, float y){
    return x*2;
}

//Ce qu'on cherche, c'est yN
float methode_ordre_un(float x0, float y0, float xN, int nb_subdivisions,float g(float, float)){
    float subdiv = (xN - x0)/(float)nb_subdivisions;
    float y_actuel = y0;
    float x_actuel = x0;
    for(int i=0; i<nb_subdivisions; i++){
        y_actuel += subdiv*g(x_actuel, y_actuel);
        x_actuel += subdiv;
    }
    return y_actuel;
}

int methode_ordre_un_fichier(float x0, float y0, float xN, int nb_subdivisions,float g(float, float)){
    float subdiv = (xN - x0)/(float)nb_subdivisions;
    float y_actuel = y0;
    float x_actuel = x0;

    FILE* fichier = fopen("coordonnees.txt", "w");
    //On vérifie si le fichier a été correctement ouvert
    if(fichier!=NULL){
        for(int i=0; i<nb_subdivisions; i++){
            fprintf(fichier, "%f %f\n", x_actuel, y_actuel);
            y_actuel += subdiv*g(x_actuel, y_actuel);
            x_actuel += subdiv;
        }
        fprintf(fichier, "%f %f", x_actuel, y_actuel);
        fclose(fichier);
        return 1;
    }
    else{
        printf("Impossible d'ouvrir le fichier %s", "coordonnees.txt");
        return 0;
    }
}

//Si j'ai bien compris le v c'est y'
float g_exponentielle_ordre2(float x, float y, float v){
    return y;
}

//Pour test encore une fois
float g_carre_ordre2(float x, float y, float v){
    return 2;
}

//Fonction trigonométrique sin(x) + cos(x)
float g_trigo(float x, float y, float v){
    return -y;
}

//Cette fonction sert à mes tests
float methode_ordre_deux(float x0, float y0, float v0, float xN, int nb_subdivisions,float g(float, float, float)){
    float subdiv = (xN - x0)/(float)nb_subdivisions;
    float y_actuel = y0;
    float x_actuel = x0;
    float v_actuel = v0;
    for(int i=0; i<nb_subdivisions; i++){
        //Observation : changer l'ordre du v et du y garde la même imprécision, mais en excès si le v est avant, et en défaut si le v est après.
        v_actuel += subdiv*g(x_actuel, y_actuel, v_actuel);
        y_actuel += subdiv*v_actuel;
        x_actuel += subdiv;
    }
    return y_actuel;
}

int methode_ordre_deux_fichier(float x0, float y0, float v0, float xN, int nb_subdivisions,float g(float, float, float)){
    float subdiv = (xN - x0)/(float)nb_subdivisions;
    float y_actuel = y0;
    float x_actuel = x0;
    float v_actuel = v0;

    FILE* fichier = fopen("coordonnees2.txt", "w");
    //On vérifie si le fichier a été correctement ouvert
    if(fichier!=NULL){
        for(int i=0; i<nb_subdivisions; i++){
            fprintf(fichier, "%f %f\n", x_actuel, y_actuel);
            y_actuel += subdiv*v_actuel;
            v_actuel += subdiv*g(x_actuel, y_actuel, v_actuel);
            x_actuel += subdiv;
        }
        fprintf(fichier, "%f %f", x_actuel, y_actuel);
        fclose(fichier);
        return 1;
    }
    else{
        printf("Impossible d'ouvrir le fichier %s", "coordonnees2.txt");
        return 0;
    }
}

//Quelle fonction dérivée deux fois donne son opposé ?
//La fonction cosinus !!! Merci la collaboration Swan / Thibaut
//En partant de là on a fouillé dans les fonctions trigonométriques pour trouver une fonction qui correspond à tous les critères
//f(x) = sin(x) + cos(x)
//Reste à déterminer si la fonction methode_ordre_deux_fichier fait bien son boulot
//Apparemment oui, elle fonctionne pour la fonction carre, et la courbe obtenue avec g_trigo correspond à la courbe f(x)=sin(x)+cos(x)

int main(){
    printf("%f",integrale_Rectangles(0.0f, 1.0f, 100, fonction));
    printf("\n");
    printf("%f",integrale_Trapezes(0.0f, 1.0f, 10, fonction));
    printf("\n");
    printf("%i",methode_ordre_un_fichier(0.0f, 1.0f, 5.0f, 1000, g_exponentielle));
    printf("\n");
    printf("%f",methode_ordre_deux(0.0f, 0.0f, 0.0f, 12.0f, 10000, g_carre_ordre2));
    printf("\n");
    printf("%i",methode_ordre_deux_fichier(0.0f, 1.0f, 1.0f, 12.0f, 10000, g_trigo));
    printf("\n");
    return 0;
}