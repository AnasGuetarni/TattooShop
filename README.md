# TattooShop

Cahier des charges
------------------

Le cahier des charges à remplir est le suivant :
1. Chaque client et chaque tatoueur est modélisé par un thread.
2. Les temps de promenade et pour réaliser un tatoo sont aléatoires. Ces temps sont tirés aléatoirement dans les intervalles **[WALK_MIN_T, WALK_MAX_T] et [TATOO_MIN_T, TATOO_MAX_T]** définis en constantes (ou defines).
3. Le programme se termine lorsque le nombre de tatouages spécifiés par l’utilisateur est atteint. Les satistiques suivantes devront être affichées sur **stderr**, avec **fprintf** :<br/>
  ◦Combien de clients ont été tatoués par chaque tatoueur.<br/>
  ◦Combien de fois chaque client a été tatoué.<br/>
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

----------
1 client = 1 thread
1 tatoueur = 1 thread

<hr/>

 Processus client :
* Balade pendant un certain temps
* Entre dans le salon :<br/>
	* Si il n’y a plus de places dans la salle d’attente, sort et part en balade<br/>
	* Si il y a de la place s’assoit dans la salle d’attente<br/>

Processus tatoueur<br/>
* Attend qu’un tatoueur se libère <br/>
* Installe le client sur le fauteuil et le tatoue, puis retourne en balade<br/>

Paramètres de l’application:<br/>
* Nombres de tatouages a réaliser<br/>
* Nombres de clients<br/>
* Nombre de tatoueurs<br/>
* Nombre de sieges disponibles<br/>

Fonctions a utiliser :<br/>
* *nanosleep* pour le temps de balade<br/>
* *assert* pour les bugs <br/>
* *rand_r* pour l’aléatoire<br/>

Exemple (5 tatouages - 3 clients - 2 tatoueurs - 2 sieges dispo)<br/>
* client1 rentre<br/>
* client2 rentre<br/>
* client3 rentre<br/>
* client1 s’assoie dans la salle d’attente<br/>
	* va se faire tatouer <br/>
	* libère une place dans la salle d’attente<br/>
* client2 s’assoie dans la salle d’attente<br/>
	* va se faire tatouer<br/>
	* libère une place dans la salle d’attente<br/>
* client3 s’assoie dans la salle d’attente<br/>
	* en attente de se faire tatouer<br/>
* client2 finit de se faire tatouer<br/>
	* sors en balade<br/>
* client3 va se faire tatouer<br/>
	* libère une place dans la salle d’attente<br/>
* client1 finit de se faire tatouer<br/>
	* sors en balade<br/>
* client2 rentre<br/>
* client2 s’assoie dans la salle d’attente<br/>
	* va se faire tatouer<br/>
	* libère une place dans la salle d’attente<br/>
* client1 rentre<br/>
* client1 s’assoie dans la salle d’attente<br/>
	* en attente de se faire tatouer<br/>
* client3 finit de se faire tatouer<br/>
	* sors en balade<br/>
* client2 finit de se faire tatouer<br/>
	* sors en balade<br/>
* client1 finit de se faire tatouer<br/>
	* sors en balade<br/>
* Arrêt du programme<br/>

Il doit alors y avoir :<br/>
* une salle d’attente qui supporte un maximum de places disponibles <br/>
* des sieges qui supporte un maximum de places disponibles<br/>
* Chaque temps de promenade est tiré aléatoirement <br/>
* Lorsque tous les tatouages ont été réalisés, on arrête le programme<br/>
* Statistiques<br/>
	* Combien de clients on été tatoués par chaque tatoueur<br/>
	* Combien de fois chaque client a été tatoué<br/>
* Clients servis dans l’ordre d’arrivée -> Liste avec id<br/>
* Lorsqu’un client se fait tatouer il est en attente passive et c’est le tatoueur qui le débloque une fois le tattoo terminé<br/>
* Aucune attente active <br/>
* Rendre la description de l’implémentation, ses étapes ainsi que toutes les informations pertinentes<br/>


