# TattooShop

Cahier des charges
------------------

Le cahier des charges à remplir est le suivant :
1. Chaque client et chaque tatoueur est modélisé par un thread.
2. Les temps de promenade et pour réaliser un tatoo sont aléatoires. Ces temps sont tirés aléatoirement dans les intervalles **[WALK_MIN_T, WALK_MAX_T] et [TATOO_MIN_T, TATOO_MAX_T]** définis en constantes (ou defines).
3. Le programme se termine lorsque le nombre de tatouages spécifiés par l’utilisateur est atteint. Les satistiques suivantes devront être affichées sur **stderr**, avec **fprintf** :
  ◦Combien de clients ont été tatoués par chaque tatoueur.
  ◦Combien de fois chaque client a été tatoué.
4. Le nombre de tatouages à réaliser, le nombre de clients, le nombre de tatoueurs, et le nombre de sièges sont passés au programme sur la ligne de commande.
5. Les clients sont servis selon leur ordre d’arrivée dans la salle d’attente (premier arrivé, premier servi).
6. Lorsqu'un client se fait tatouer, il est synchrone avec le tatoueur ; autrement dit, le client est en attente passive pendant la durée du tatoo et c’est le tatoueur qui signale le client une fois le tatoo terminé.
7. Votre programme ne doit comporter aucune attente active et seules les primitives de synchronisation étudiées en cours sont permises.
8. Pour la temporisation, utilisez la fonction POSIX **nanosleep** (cf. man nanosleep).
9. Utilisez des **assert** où cela est appoprié afin de détecter des bugs potentiels.
10. La description de l’implémentation, ses différentes étapes, et toute autre information pertinente doivent figurer dans le code rendu.

Informations utiles 
-------------------

1. N’hésitez pas à afficher des messages afin de valider le comportement de votre programme.
2. Pour tester la présence de deadlocks, il est conseillé de compiler votre programme avec l’option -O3  (optimisation pour la vitesse), puis de l’exécuter avec des temps nuls et aucun affichage. Pour supprimer les affichages liés à printf, vous pouvez utiliser la macro suivante :  **#define printf(fmt, ...) (void)(0)**
3. Attention, la fonction rand n’est pas thread-safe. Pensez à utiliser la fonction rand_r dans un contexte multi-threadé.

Notation
--------

Conception, conformité au cahier des charges et simplicité  |  20 %<br/>
Exécution et fonctionnement                                 |  25 %<br/>
Code                                                        |  15 %<br/>
Documentation et en-têtes des fonctions                     |  20 %<br/>
Commentaires au niveau du code                              |  10 %<br/>
Robustesse                                                  |  10 %<br/>

