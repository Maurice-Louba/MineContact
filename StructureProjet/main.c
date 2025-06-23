#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct Contact {
    char nom[50];
    char tel[50];
    char mail[50];
    struct Contact* suiv;
} Contact;

typedef struct LettreContact {
    char lettre;
    Contact* contacts; // sous-liste triée
    struct LettreContact* suiv;
} LettreContact;

typedef struct {
    LettreContact* debut;
} Carnet;

// Fonctions de validation
bool est_alpha(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

bool est_numerique(const char* str) {
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool valider_email(const char* email) {
    int at_count = 0;
    int dot_count = 0;
    int at_pos = -1;
    int dot_pos = -1;
    int len = strlen(email);

    // Vérifier la présence d'un @ et d'un .
    for (int i = 0; i < len; i++) {
        if (email[i] == '@') {
            at_count++;
            at_pos = i;
        } else if (email[i] == '.') {
            dot_count++;
            dot_pos = i;
        }
    }

    // Doit avoir exactement un @ et au moins un .
    if (at_count != 1 || dot_count < 1) {
        return false;
    }

    // Le @ ne peut pas être au début ou à la fin
    if (at_pos == 0 || at_pos == len - 1) {
        return false;
    }

    // Le . ne peut pas être juste après le @
    if (dot_pos == at_pos + 1) {
        return false;
    }

    // Le . doit être après le @
    if (dot_pos < at_pos) {
        return false;
    }

    // Le . ne peut pas être à la fin
    if (dot_pos == len - 1) {
        return false;
    }

    return true;
}

// Initialise le carnet
void initialiser(Carnet* carnet) {
    carnet->debut = NULL;
}

// Insertion triée dans une sous-liste de contacts
Contact* inserer_contact_trie(Contact* tete, Contact* nouveau) {
    if (tete == NULL || strncmp(nouveau->nom, tete->nom, 2) < 0) {
        nouveau->suiv = tete;
        return nouveau;
    }

    Contact* courant = tete;
    while (courant->suiv != NULL && strncmp(nouveau->nom, courant->suiv->nom, 2) > 0) {
        courant = courant->suiv;
    }

    nouveau->suiv = courant->suiv;
    courant->suiv = nouveau;

    return tete;
}

// Ajoute un nouveau contact dans le carnet avec vérifications strictes
void ajouter_contact(Carnet* carnet) {
    Contact* nouveau = (Contact*)malloc(sizeof(Contact));
    if (!nouveau) {
        printf("Erreur d'allocation mémoire ❌\n");
        return;
    }

    // Saisie et validation du nom
    bool nom_valide = false;
    do {
        printf("Nom (lettres seulement, pas de caractères spéciaux) : ");
        scanf(" %49s", nouveau->nom);

        if (est_alpha(nouveau->nom)) {
            nom_valide = true;
        } else {
            printf("❌ Nom invalide. Utilisez uniquement des lettres.\n");
        }
    } while (!nom_valide);

    // Saisie et validation du téléphone
    bool tel_valide = false;
    do {
        printf("Téléphone (exactement 10 chiffres) : ");
        scanf(" %49s", nouveau->tel);

        if (strlen(nouveau->tel) == 10 && est_numerique(nouveau->tel)) {
            tel_valide = true;
        } else {
            printf("❌ Téléphone invalide. Doit contenir exactement 10 chiffres.\n");
        }
    } while (!tel_valide);

    // Saisie et validation de l'email
    bool mail_valide = false;
    do {
        printf("E-mail (format: xxx@yyy.zz) : ");
        scanf(" %49s", nouveau->mail);

        if (valider_email(nouveau->mail)) {
            mail_valide = true;
        } else {
            printf("❌ E-mail invalide. Format attendu: xxx@yyy.zz\n");
        }
    } while (!mail_valide);

    nouveau->suiv = NULL;

    char lettre = nouveau->nom[0];

    // Recherche de la lettre correspondante
    LettreContact* courant = carnet->debut;
    LettreContact* precedent = NULL;

    while (courant && courant->lettre < lettre) {
        precedent = courant;
        courant = courant->suiv;
    }

    if (courant && courant->lettre == lettre) {
        // Ajout dans la sous-liste triée
        courant->contacts = inserer_contact_trie(courant->contacts, nouveau);
    } else {
        // Crée une nouvelle catégorie pour la lettre
        LettreContact* nouvelle_lettre = (LettreContact*)malloc(sizeof(LettreContact));
        if (!nouvelle_lettre) {
            printf("Erreur d'allocation mémoire ❌\n");
            free(nouveau);
            return;
        }

        nouvelle_lettre->lettre = lettre;
        nouvelle_lettre->contacts = nouveau;
        nouvelle_lettre->suiv = courant;

        if (precedent == NULL) {
            carnet->debut = nouvelle_lettre;
        } else {
            precedent->suiv = nouvelle_lettre;
        }
    }

    printf("✅ Contact ajouté avec succès !\n");
}

// Affiche tous les contacts triés
void afficher_contacts(const Carnet* carnet) {
    LettreContact* courant_lettre = carnet->debut;

    while (courant_lettre) {
        printf("\n🔠 Lettre %c :\n", courant_lettre->lettre);
        Contact* c = courant_lettre->contacts;
        while (c) {
            printf("   👤 Nom : %s | 📞 Tel : %s | 📧 Mail : %s\n", c->nom, c->tel, c->mail);
            c = c->suiv;
        }
        courant_lettre = courant_lettre->suiv;
    }
}

void supprimer_contact(Carnet* carnet) {
    char nom[50];
    printf("Entrez le nom du contact à supprimer : ");
    scanf(" %49s", nom);

    char lettre = nom[0];

    LettreContact* courant_lettre = carnet->debut;
    LettreContact* lettre_precedente = NULL;

    // Recherche de la bonne lettre
    while (courant_lettre && courant_lettre->lettre != lettre) {
        lettre_precedente = courant_lettre;
        courant_lettre = courant_lettre->suiv;
    }

    if (!courant_lettre) {
        printf("❌ Aucun contact trouvé avec la lettre %c.\n", lettre);
        return;
    }

    Contact* courant_contact = courant_lettre->contacts;
    Contact* contact_precedent = NULL;

    int compteur = 0;

    // On compte les correspondances au nom
    Contact* temp = courant_contact;
    while (temp) {
        if (strcmp(temp->nom, nom) == 0) {
            compteur++;
        }
        temp = temp->suiv;
    }

    if (compteur == 0) {
        printf("❌ Aucun contact trouvé avec le nom %s.\n", nom);
        return;
    }

    char tel[50];

    if (compteur > 1) {
        // Si doublon, demander plus de précision
        printf("⚠️ Il y a %d contacts avec ce nom.\n", compteur);
        printf("Entrez aussi le numéro de téléphone : ");
        scanf(" %49s", tel);
    }

    courant_contact = courant_lettre->contacts;
    contact_precedent = NULL;

    // Suppression basée sur nom (et tel si nécessaire)
    while (courant_contact) {
        if (strcmp(courant_contact->nom, nom) == 0 &&
            (compteur == 1 || strcmp(courant_contact->tel, tel) == 0)) {

            // Suppression du contact trouvé
            if (contact_precedent == NULL) {
                courant_lettre->contacts = courant_contact->suiv;
            } else {
                contact_precedent->suiv = courant_contact->suiv;
            }

            free(courant_contact);
            printf("✅ Contact supprimé avec succès.\n");

            // Vérifie si la sous-liste est vide
            if (courant_lettre->contacts == NULL) {
                if (lettre_precedente == NULL) {
                    carnet->debut = courant_lettre->suiv;
                } else {
                    lettre_precedente->suiv = courant_lettre->suiv;
                }
                free(courant_lettre);
            }

            return; // sortie après suppression
        }

        contact_precedent = courant_contact;
        courant_contact = courant_contact->suiv;
    }

    printf("❌ Aucun contact correspondant nom + téléphone n'a été trouvé.\n");
}

void chercher_contact(const Carnet* carnet) {
    char nom_recherche[50];
    int trouve = 0;

    printf("🔍 Entrez le nom complet à rechercher : ");
    scanf(" %49s", nom_recherche);

    char lettre = nom_recherche[0];

    LettreContact* courant_lettre = carnet->debut;

    // Parcours des lettres jusqu'à la bonne
    while (courant_lettre && courant_lettre->lettre != lettre) {
        courant_lettre = courant_lettre->suiv;
    }

    if (!courant_lettre) {
        printf("❌ Aucun contact trouvé avec la lettre %c.\n", lettre);
        return;
    }

    Contact* courant_contact = courant_lettre->contacts;

    // Parcours de la sous-liste
    while (courant_contact) {
        if (strcmp(courant_contact->nom, nom_recherche) == 0) {
            printf("\n👤 Contact trouvé :\n");
            printf("   Nom  : %s\n", courant_contact->nom);
            printf("   Tel  : %s\n", courant_contact->tel);
            printf("   Mail : %s\n", courant_contact->mail);
            trouve = 1;
        }
        courant_contact = courant_contact->suiv;
    }

    if (!trouve) {
        printf("❌ Aucun contact trouvé avec le nom \"%s\".\n", nom_recherche);
    }
}

void chercher_par_premiere_lettre(const Carnet* carnet) {
    char lettre;
    int trouve = 0;

    printf("🔠 Entrez la première lettre des contacts à rechercher : ");
    scanf(" %c", &lettre);

    // Mettre en majuscule si besoin (optionnel)
    if (lettre >= 'a' && lettre <= 'z') {
        lettre -= 32;
    }

    LettreContact* courant_lettre = carnet->debut;

    // Parcours des lettres
    while (courant_lettre) {
        if (courant_lettre->lettre == lettre) {
            Contact* courant = courant_lettre->contacts;
            printf("\n📋 Contacts commençant par '%c' :\n", lettre);

            while (courant) {
                printf(" - 👤 Nom : %s | 📞 %s | 📧 %s\n",
                       courant->nom, courant->tel, courant->mail);
                courant = courant->suiv;
                trouve = 1;
            }
            break;
        }
        courant_lettre = courant_lettre->suiv;
    }

    if (!trouve) {
        printf("❌ Aucun contact trouvé commençant par la lettre '%c'.\n", lettre);
    }
}

void chercher_par_deux_premieres_lettres(const Carnet* carnet) {
    char prefixe[3]; // 2 lettres + '\0'
    int trouve = 0;

    printf("🔠 Entrez les deux premières lettres du nom à rechercher (ex: Ma) : ");
    scanf(" %2s", prefixe); // lecture de 2 lettres max

    // Mettre en majuscules si nécessaire (optionnel)
    for (int i = 0; i < 2; i++) {
        if (prefixe[i] >= 'a' && prefixe[i] <= 'z') {
            prefixe[i] -= 32;
        }
    }

    LettreContact* courant_lettre = carnet->debut;

    while (courant_lettre) {
        Contact* courant_contact = courant_lettre->contacts;

        while (courant_contact) {
            if (strncmp(courant_contact->nom, prefixe, 2) == 0) {
                if (!trouve) {
                    printf("\n📋 Contacts commençant par \"%s\" :\n", prefixe);
                }

                printf(" - 👤 Nom : %s | 📞 %s | 📧 %s\n",
                       courant_contact->nom,
                       courant_contact->tel,
                       courant_contact->mail);

                trouve = 1;
            }
            courant_contact = courant_contact->suiv;
        }

        courant_lettre = courant_lettre->suiv;
    }

    if (!trouve) {
        printf("❌ Aucun contact trouvé avec les lettres \"%s\".\n", prefixe);
    }
}

void modifier_contact(Carnet* carnet) {
    char nom[50];
    char tel[50];
    int compteur = 0;
    int trouve = 0;

    printf("✏️ Entrez le nom du contact à modifier : ");
    scanf(" %49s", nom);

    char lettre = nom[0];
    LettreContact* lettre_courante = carnet->debut;

    // Trouver la lettre
    while (lettre_courante && lettre_courante->lettre != lettre) {
        lettre_courante = lettre_courante->suiv;
    }

    if (!lettre_courante) {
        printf("❌ Aucun contact trouvé avec la lettre %c.\n", lettre);
        return;
    }

    // Compter les correspondances
    Contact* temp = lettre_courante->contacts;
    while (temp) {
        if (strcmp(temp->nom, nom) == 0) {
            compteur++;
        }
        temp = temp->suiv;
    }

    if (compteur == 0) {
        printf("❌ Aucun contact avec le nom \"%s\".\n", nom);
        return;
    }

    if (compteur > 1) {
        printf("⚠️ Plusieurs contacts avec ce nom existent.\n");
        printf("Entrez le numéro de téléphone pour identifier le bon : ");
        scanf(" %49s", tel);
    }

    Contact* courant = lettre_courante->contacts;

    // Recherche précise
    while (courant) {
        if (strcmp(courant->nom, nom) == 0 &&
            (compteur == 1 || strcmp(courant->tel, tel) == 0)) {

            printf("\n👤 Contact trouvé :\n");
            printf("   Nom  : %s\n", courant->nom);
            printf("   Tel  : %s\n", courant->tel);
            printf("   Mail : %s\n", courant->mail);

            // Modification
            printf("\nQue voulez-vous modifier ?\n");
            printf("1. Nom\n2. Téléphone\n3. Mail\n0. Annuler\nChoix : ");
            int choix;
            scanf("%d", &choix);

            switch (choix) {
                case 1: {
                    bool nom_valide = false;
                    do {
                        printf("Nouveau nom (lettres seulement) : ");
                        scanf(" %49s", courant->nom);

                        if (est_alpha(courant->nom)) {
                            nom_valide = true;
                        } else {
                            printf("❌ Nom invalide. Utilisez uniquement des lettres.\n");
                        }
                    } while (!nom_valide);
                    break;
                }
                case 2: {
                    bool tel_valide = false;
                    do {
                        printf("Nouveau téléphone (exactement 10 chiffres) : ");
                        scanf(" %49s", courant->tel);

                        if (strlen(courant->tel) == 10 && est_numerique(courant->tel)) {
                            tel_valide = true;
                        } else {
                            printf("❌ Téléphone invalide. Doit contenir exactement 10 chiffres.\n");
                        }
                    } while (!tel_valide);
                    break;
                }
                case 3: {
                    bool mail_valide = false;
                    do {
                        printf("Nouveau mail (format: xxx@yyy.zz) : ");
                        scanf(" %49s", courant->mail);

                        if (valider_email(courant->mail)) {
                            mail_valide = true;
                        } else {
                            printf("❌ E-mail invalide. Format attendu: xxx@yyy.zz\n");
                        }
                    } while (!mail_valide);
                    break;
                }
                case 0:
                    printf("❎ Modification annulée.\n");
                    return;
                default:
                    printf("❌ Choix invalide.\n");
                    return;
            }

            printf("✅ Contact modifié avec succès !\n");
            trouve = 1;
            break;
        }
        courant = courant->suiv;
    }

    if (!trouve) {
        printf("❌ Aucun contact trouvé avec ce nom et ce numéro.\n");
    }
}

int main() {
    system("chcp 65001");
    Carnet monCarnet;
    initialiser(&monCarnet);

    int choix;

    do {
        printf("\n==============================\n");
        printf("📇 Menu Gestion de Contacts\n");
        printf("==============================\n");
        printf("1. Ajouter un contact\n");
        printf("2. Supprimer un contact\n");
        printf("3. Afficher tous les contacts\n");
        printf("4. Rechercher un contact\n");
        printf("5. Modifier un contact\n");
        printf("0. Quitter\n");
        printf("------------------------------\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_contact(&monCarnet);
                break;

            case 2:
                supprimer_contact(&monCarnet);
                break;

            case 3:
                afficher_contacts(&monCarnet);
                break;

            case 4: {
                int sous_choix;
                printf("\n===== 🔍 Menu Recherche =====\n");
                printf("1. Rechercher par nom complet\n");
                printf("2. Rechercher par première lettre\n");
                printf("3. Rechercher par deux premières lettres\n");
                printf("0. Retour au menu principal\n");
                printf("------------------------------\n");
                printf("Votre choix : ");
                scanf("%d", &sous_choix);

                switch (sous_choix) {
                    case 1:
                        chercher_contact(&monCarnet);
                        break;
                    case 2:
                        chercher_par_premiere_lettre(&monCarnet);
                        break;
                    case 3:
                        chercher_par_deux_premieres_lettres(&monCarnet);
                        break;
                    case 0:
                        break;
                    default:
                        printf("❌ Choix invalide dans le sous-menu.\n");
                }
                break;
            }

            case 5:
                modifier_contact(&monCarnet);
                break;

            case 0:
                printf("👋 Au revoir !\n");
                break;

            default:
                printf("❌ Choix invalide. Veuillez réessayer.\n");
        }

    } while (choix != 0);

    return 0;
}
