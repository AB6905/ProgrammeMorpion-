#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class Morpion {  //Création de la classe prinicpale

public:
    char grille[3][3];  //Initialisation de la grille
    char joueur;  //initialisation du joueur
    int lastRow, lastCol; //plus tard pour IA2

    Morpion() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                grille[i][j] = ' ';  //Création de la grille 2D à 3 lignes et 3 colonnes remplies d'espace pour chaque position
        joueur = 'X';
        lastRow = -1;
        lastCol = -1;  //plus tard pour l'IA 2
    }

    void afficherGrille() {
        cout << "\n  1 2 3\n";
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < 3; j++) {
                cout << grille[i][j];
                if (j < 2) cout << "|";
            }
            cout << "\n";
            if (i < 2) cout << "  -+-+-\n";  //Affichage des lignes horizontales et colonnes verticales pour les delimitations
        }
        cout << "\n";
    }

    bool jouer(int l, int c) {
        if (l < 0 || l > 2 || c < 0 || c > 2) return false;  //Empeche le joueur de jouer hors grille
        if (grille[l][c] != ' ') return false;  ////Ne peut pas jouer sur un 'X' ou 'O'
        grille[l][c] = joueur;  //Place le symbole du joueur actuel dans la case
        if (joueur == 'O') { // IA2 se souvient de son dernier coup
            lastRow = l;
            lastCol = c;
        }
        return true;
    }

    bool victoire() {
        for (int i = 0; i < 3; i++)
            if (grille[i][0] == joueur && grille[i][1] == joueur && grille[i][2] == joueur)  //Vérifie les lignes
                return true;
        for (int j = 0; j < 3; j++)
            if (grille[0][j] == joueur && grille[1][j] == joueur && grille[2][j] == joueur) //Verifie les colonnes
                return true;
        if (grille[0][0] == joueur && grille[1][1] == joueur && grille[2][2] == joueur) return true;
        if (grille[0][2] == joueur && grille[1][1] == joueur && grille[2][0] == joueur) return true;  //Vérifie les deux diagonales
        return false;
    }

    bool nul() {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (grille[i][j] == ' ')  //Si une case est encore vide → la partie n’est pas finie, donc pas encore match nul
                    return false;
        return true;  //Aucune case vide trouvée → toutes les cases remplies -> Match nul car victoire vérifiée avant
    }

    void changerJoueur() {
        if (joueur == 'X')
            joueur = 'O';
        else
            joueur = 'X';  //Si 'X' joue alors c'est le tour de 'O'
    }

    // IA1 : joue aléatoire
    void jouerIA1() {
        int l, c;
        do {
            l = rand() % 3;
            c = rand() % 3;
        } while (!jouer(l, c));  //Joue un coup aléatoire entre ligne 1 et 3 et colonne 1 et 3
    }

    // IA2 : joue proche de son dernier coup si possible
    void jouerIA2() {
        int l = lastRow;
        int c = lastCol;

        // Essayer les cases autour de la dernière position
        int directions[8][2] = { {-1,0}, {1,0}, {0,-1}, {0,1}, {-1,-1}, {-1,1}, {1,-1}, {1,1} };  //Création de la variable direction
        for (int i = 0; i < 8; i++) {
            int nl = l + directions[i][0];
            int nc = c + directions[i][1];  //Cela teste toutes les cases adjacentes autour de (l, c) pour savoir si elles sont vides et pouvoir y jouer.
            if (nl >=0 && nl < 3 && nc >=0 && nc < 3 && grille[nl][nc] == ' ') {  //Vérifie que la case (nl, nc) est dans la grille 3x3, La case doit contenir ' ' (vide)
                jouer(nl, nc);  //Appelle la fonction jouer() pour placer le symbole du joueur actuel sur la case (nl, nc)
                return;  //Une fois que l’IA a joué, elle ne teste plus les autres cases
            }
        }

        int rl, rc;
        do {
            rl = rand() % 3;  // Si aucune case autour n'est vide, joue aléatoire
            rc = rand() % 3;  //Ligne et colonne random entre 1 et 3
        } while (!jouer(rl, rc));  //vrai si le coup n’est pas valide, Donc la boucle continue tant que la case est occupée
    }
};

// Jeu joueur contre joueur
void jouerJoueurContreJoueur() {
    Morpion jeu;
    int ligne, colonne;
    while (true) {
        jeu.afficherGrille();  //crée une grille pour joueur contre joueur
        cout << "Joueur " << jeu.joueur << " - ligne et colonne (1-3) : ";
        cin >> ligne >> colonne;  //demande la ligne et la colonne dans laquelle le joueur X veut jouer

        if (!jeu.jouer(ligne - 1, colonne - 1)) {  //Eviter un jeu hors grille
            cout << "Case invalide. Réessayez.\n";
            continue;
        }

        if (jeu.victoire()) { jeu.afficherGrille(); cout << "Le joueur " << jeu.joueur << " a gagné !\n"; break; }  //affiche la grille et fais un message de victoire
        if (jeu.nul()) { jeu.afficherGrille(); cout << "Match nul !\n"; break; }  //affiche la grille et ecrit match nul dans le cas d'un match nul

        jeu.changerJoueur();
    }
}

// Jeu joueur contre ordinateur
void jouerContreOrdi() {
    Morpion jeu;
    int ligne, colonne;
    cout << "Vous êtes X, l'ordinateur est O.\n";

    while (true) {
        // Tour du joueur
        jeu.afficherGrille();
        cout << "Votre tour - ligne et colonne (1-3) : ";
        cin >> ligne >> colonne;  //Demande la ligne et la colonne voulue par le joueur

        if (!jeu.jouer(ligne - 1, colonne - 1)) { cout << "Case invalide. Réessayez.\n"; continue; }  //verifie la disponibilité de la case
        if (jeu.victoire()) { jeu.afficherGrille(); cout << "Vous avez gagné !\n"; break; }  //message de victoire
        if (jeu.nul()) { jeu.afficherGrille(); cout << "Match nul !\n"; break; }  //message de match nul

        // Tour ordinateur
        jeu.changerJoueur();
        cout << "L'ordinateur joue..."<<endl;
        jeu.jouerIA2();  //fait jouer l'ordinateur IA2, celle qui joue ses coups à coté des anciens

        if (jeu.victoire()) { jeu.afficherGrille(); cout << "L'ordinateur a gagné !"<<endl; break; }  //message victoire de l'ordi
        if (jeu.nul()) { jeu.afficherGrille(); cout << "Match nul !\n"; break; }  //message match nul

        jeu.changerJoueur();
    }
}

// IA1 vs IA2
void jouerIAContreIA() {
    Morpion jeu;
    cout << "IA1 (X) vs IA2 (O)"<<endl;

    // IA1 joue la première case aléatoire
    jeu.jouerIA1();  //IA1 qui joue des coups aléatoires
    jeu.changerJoueur();  //change le joueur aprrès son coup

    while (true) {
    jeu.afficherGrille();
    cout << "Tour " << jeu.joueur << "\n";

    cout << "Appuyez sur Entrée pour que l'IA joue...";
    cin.ignore(); // ignore le reste de la ligne précédente
    cin.get();   // attend l'Entrée

    if (jeu.joueur == 'X') {
        jeu.jouerIA1();
    } else {
        jeu.jouerIA2();  //vérifie à qui c'est le tour
    }

    if (jeu.victoire()) {
        jeu.afficherGrille();
        cout << "Le joueur " << jeu.joueur << " a gagné !"<<endl;
        break;
    }

    if (jeu.nul()) {
        jeu.afficherGrille();
        cout << "Match nul !"<<endl;
        break;  //Match nul = fin du match
    }

    jeu.changerJoueur();
}
}

int main() {
    srand(time(NULL));
    int choix;

    cout << "Jeu du Morpion"<<endl;
    cout << "1. Joueur contre joueur"<<endl;
    cout << "2. Joueur contre ordinateur"<<endl;
    cout << "3. IA1 (X) vs IA2 (O)"<<endl;
    cout << "Choisissez : ";
    cin >> choix;

    if (choix == 1) jouerJoueurContreJoueur();
    else if (choix == 2) jouerContreOrdi();
    else if (choix == 3) jouerIAContreIA();
    else cout << "Choix invalide."<<endl;

    return 0;
}
