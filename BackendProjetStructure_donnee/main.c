#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"  // inclure cJSON

#define PORT 8888

// --- Ta structure de contact (simplifiée ici) ---

typedef struct Contact {
    char nom[50];
    char tel[50];
    char mail[50];
    struct Contact* suiv;
} Contact;

typedef struct LettreContact {
    char lettre;
    Contact* contacts;
    struct LettreContact* suiv;
} LettreContact;

typedef struct {
    LettreContact* debut;
} Carnet;

Carnet monCarnet;

void initialiser(Carnet* carnet) {
    carnet->debut = NULL;
}

// Ajout contact dans sous-liste triée (par nom)
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

void ajouter_contact_api(Carnet* carnet, const char* nom, const char* tel, const char* mail) {
    Contact* nouveau = (Contact*)malloc(sizeof(Contact));
    if (!nouveau) return;

    strncpy(nouveau->nom, nom, 49);
    nouveau->nom[49] = '\0';
    strncpy(nouveau->tel, tel, 49);
    nouveau->tel[49] = '\0';
    strncpy(nouveau->mail, mail, 49);
    nouveau->mail[49] = '\0';
    nouveau->suiv = NULL;

    char lettre = nouveau->nom[0];
    if (lettre >= 'a' && lettre <= 'z') lettre -= 32; // Majuscule

    LettreContact* courant = carnet->debut;
    LettreContact* precedent = NULL;

    while (courant && courant->lettre < lettre) {
        precedent = courant;
        courant = courant->suiv;
    }

    if (courant && courant->lettre == lettre) {
        courant->contacts = inserer_contact_trie(courant->contacts, nouveau);
    } else {
        LettreContact* nouvelle_lettre = (LettreContact*)malloc(sizeof(LettreContact));
        if (!nouvelle_lettre) {
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
}

// Conversion carnet -> JSON (string)
char* contacts_to_json(const Carnet* carnet) {
    cJSON* root = cJSON_CreateArray();

    LettreContact* courant_lettre = carnet->debut;
    while (courant_lettre) {
        Contact* c = courant_lettre->contacts;
        while (c) {
            cJSON* item = cJSON_CreateObject();
            cJSON_AddStringToObject(item, "nom", c->nom);
            cJSON_AddStringToObject(item, "tel", c->tel);
            cJSON_AddStringToObject(item, "mail", c->mail);
            cJSON_AddItemToArray(root, item);
            c = c->suiv;
        }
        courant_lettre = courant_lettre->suiv;
    }

    char* json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // à free après utilisation
}

// Suppression contact API (nom + tel)
int supprimer_contact_api(Carnet* carnet, const char* nom, const char* tel) {
    char lettre = nom[0];
    if (lettre >= 'a' && lettre <= 'z') lettre -= 32; // Majuscule

    LettreContact* courant_lettre = carnet->debut;
    LettreContact* lettre_precedente = NULL;

    while (courant_lettre && courant_lettre->lettre != lettre) {
        lettre_precedente = courant_lettre;
        courant_lettre = courant_lettre->suiv;
    }
    if (!courant_lettre) return 0;

    Contact* courant_contact = courant_lettre->contacts;
    Contact* contact_precedent = NULL;

    while (courant_contact) {
        if (strcmp(courant_contact->nom, nom) == 0 && strcmp(courant_contact->tel, tel) == 0) {
            // Supprimer contact
            if (contact_precedent == NULL) {
                courant_lettre->contacts = courant_contact->suiv;
            } else {
                contact_precedent->suiv = courant_contact->suiv;
            }
            free(courant_contact);

            // Supprimer lettre si plus de contacts
            if (courant_lettre->contacts == NULL) {
                if (lettre_precedente == NULL) {
                    carnet->debut = courant_lettre->suiv;
                } else {
                    lettre_precedente->suiv = courant_lettre->suiv;
                }
                free(courant_lettre);
            }
            return 1;
        }
        contact_precedent = courant_contact;
        courant_contact = courant_contact->suiv;
    }
    return 0;
}

// --- Gestion des requêtes HTTP ---

struct RequestData {
    char* data;
    size_t size;
};

int repondre(void* cls,
             struct MHD_Connection* connection,
             const char* url,
             const char* method,
             const char* version,
             const char* upload_data,
             size_t* upload_data_size,
             void** con_cls) {

    struct MHD_Response* response;
    int ret;

    // Premier appel : allouer mémoire pour recevoir le corps (POST)
    if (*con_cls == NULL) {
        struct RequestData* request_data = malloc(sizeof(struct RequestData));
        request_data->data = NULL;
        request_data->size = 0;
        *con_cls = request_data;
        return MHD_YES;
    }

    struct RequestData* request_data = *con_cls;

    // Si upload_data_size > 0, on stocke le corps reçu
    if (*upload_data_size > 0) {
        request_data->data = realloc(request_data->data, request_data->size + *upload_data_size + 1);
        memcpy(request_data->data + request_data->size, upload_data, *upload_data_size);
        request_data->size += *upload_data_size;
        request_data->data[request_data->size] = '\0';
        *upload_data_size = 0;
        return MHD_YES;
    } else {
        // corps entièrement reçu, on traite la requête

        if (strcmp(method, "GET") == 0 && strcmp(url, "/contacts") == 0) {
            // Retourner tous les contacts en JSON
            char* json = contacts_to_json(&monCarnet);

            response = MHD_create_response_from_buffer(strlen(json), (void*)json, MHD_RESPMEM_MUST_FREE);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

        }
        else if (strcmp(method, "POST") == 0 && strcmp(url, "/contacts") == 0) {
            // Ajouter contact à partir JSON reçu dans request_data->data
            cJSON* root = cJSON_Parse(request_data->data);
            if (!root) {
                const char* err = "{\"error\":\"JSON invalide\"}";
                response = MHD_create_response_from_buffer(strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
                MHD_add_response_header(response, "Content-Type", "application/json");
                ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                MHD_destroy_response(response);
                MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

            } else {
                cJSON* nom = cJSON_GetObjectItem(root, "nom");
                cJSON* tel = cJSON_GetObjectItem(root, "tel");
                cJSON* mail = cJSON_GetObjectItem(root, "mail");
                if (cJSON_IsString(nom) && cJSON_IsString(tel) && cJSON_IsString(mail)) {
                    ajouter_contact_api(&monCarnet, nom->valuestring, tel->valuestring, mail->valuestring);
                    const char* succ = "{\"message\":\"Contact ajouté\"}";
                    response = MHD_create_response_from_buffer(strlen(succ), (void*)succ, MHD_RESPMEM_PERSISTENT);
                    MHD_add_response_header(response, "Content-Type", "application/json");
                    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
                    MHD_destroy_response(response);
                    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

                } else {
                    const char* err = "{\"error\":\"Champs manquants\"}";
                    response = MHD_create_response_from_buffer(strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
                    MHD_add_response_header(response, "Content-Type", "application/json");
                    ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                    MHD_destroy_response(response);
                    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

                }
                cJSON_Delete(root);
            }
        }
        else if (strcmp(method, "DELETE") == 0 && strcmp(url, "/contacts") == 0) {
            // Supprimer contact à partir JSON nom + tel dans request_data->data
            cJSON* root = cJSON_Parse(request_data->data);
            if (!root) {
                const char* err = "{\"error\":\"JSON invalide\"}";
                response = MHD_create_response_from_buffer(strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
                MHD_add_response_header(response, "Content-Type", "application/json");
                ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                MHD_destroy_response(response);
                MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

            } else {
                cJSON* nom = cJSON_GetObjectItem(root, "nom");
                cJSON* tel = cJSON_GetObjectItem(root, "tel");
                if (cJSON_IsString(nom) && cJSON_IsString(tel)) {
                    int res = supprimer_contact_api(&monCarnet, nom->valuestring, tel->valuestring);
                    if (res) {
                        const char* succ = "{\"message\":\"Contact supprimé\"}";
                        response = MHD_create_response_from_buffer(strlen(succ), (void*)succ, MHD_RESPMEM_PERSISTENT);
                        MHD_add_response_header(response, "Content-Type", "application/json");
                        ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
                        MHD_destroy_response(response);
                        MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

                    } else {
                        const char* err = "{\"error\":\"Contact non trouvé\"}";
                        response = MHD_create_response_from_buffer(strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
                        MHD_add_response_header(response, "Content-Type", "application/json");
                        ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
                        MHD_destroy_response(response);
                        MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

                    }
                } else {
                    const char* err = "{\"error\":\"Champs manquants\"}";
                    response = MHD_create_response_from_buffer(strlen(err), (void*)err, MHD_RESPMEM_PERSISTENT);
                    MHD_add_response_header(response, "Content-Type", "application/json");
                    ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                    MHD_destroy_response(response);
                    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

                }
                cJSON_Delete(root);
            }
        } else {
            // 404 Not Found
            const char* not_found = "{\"error\":\"Route non trouvée\"}";
            response = MHD_create_response_from_buffer(strlen(not_found), (void*)not_found, MHD_RESPMEM_PERSISTENT);
            MHD_add_response_header(response, "Content-Type", "application/json");
            ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);
            MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
MHD_add_response_header(response, "Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
MHD_add_response_header(response, "Access-Control-Allow-Headers", "Content-Type");

        }

        // Libérer mémoire
        free(request_data->data);
        free(request_data);
        *con_cls = NULL;

        return ret;
    }
    return MHD_YES;
}

int main() {
    initialiser(&monCarnet);

    struct MHD_Daemon* daemon = MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL, &repondre, NULL, MHD_OPTION_END);

    if (daemon == NULL) {
        printf("Erreur démarrage serveur\n");
        return 1;
    }

    printf("Serveur HTTP démarré sur http://localhost:%d\n", PORT);
    printf("Appuyez sur Entrée pour arrêter...\n");
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}

