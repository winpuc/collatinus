# Collatinus-11, notes

## vendredi 21 septembre 2018

# Format des données optionnelles chargeables et déchargeables.

- Elles sont livrées sous forme de paquet qui se décompresse dans l'espace
  personnel de l'utilisateur ;
- Dans un paquet :
  * lexique
  * traductions
  * particularités graphiques (assimilations, contractions, équivalences graphiques)
  * modèles
- Action des données optionnelles sur les données permanentes
  Les données permanentes ne sont pas déchargées au chargement des données
  optionnelles, mais elle peuvent être modifiées par elles. Liste des 
  actions des données optionnelles :
  * ajouter un lemme et ses traductions ;
  * supprimer un lemme et ses traductions (seulement si son apparition est
    ultérieure au corpus visé par le paquet) ; on garde les lemmes
    apparemment inutilisés, parce qu'un auteur est toujours susceptible
    d'employer un archaïsme.
  * modifier seulement la graphie et le modèle d'un lemme ;
  * modifier les traductions d'un lemme, quelquefois en changeant l'ordre
    d'apparition de ces traductions ;
  * supprimer des traductions (sens apparu ultérieurement au corpus visé) ;
  * ajouter des traductions sans supprimer celles des données permanentes ;
  * même principe pour les autres données : assimilations, contractions, équivalences.
  
- Problème du classement des traductions. Les dictionnaires papier suivent
  plutôt un ordre chronologique. Par exemple, /lego/ a comme premier sens
  « cueillir ». Faudrait-il adopter un ordre fréquenciel ? On peut aussi,
  garder l'ordre chronologique, et trouver le moyen d'indiquer les fréquences.

## vendredi 14 septembre 2018*

La branche master est aujourd'hui la source de la version 11.1 de
Collatinus. Elle est issue de la branche saucisson. Le merge 
de saucisson dans master étant devenu très difficile, master a été
mise à jour par copie des sources de saucisson dans le répertoire src/,
et l'état antérieur de master est archivé dans la branche vmaster.
La branche saucisson est conservée parce qu'elle conserve la trace de son
développement depuis sa création à partir de master.

Deux autres branches sont conservées :
- Medieval : Cette branche essaie de traiter simplement la variabilité des graphies médiévales.
- tagPlus : Tentative de construire sur le tagueur un générateur d'arbres syntaxiques (non-aboutie en septembre 2018).



## Dimanche 2 octobre 2016 BRANCHE SERVEUR

J'ai repris le serveur TCP que j'avais implémenté dans C10.3 et je l'ai installé dans C11.
Pour l'interroger, on utilise un client qui envoie une requête au serveur
et affiche la réponse.

La syntaxe est '[commande] [texte]' ou '[commande] -f nom_de_fichier'.

Par défaut (sans commande), on obtient la scansion du texte.

Les commandes possibles sont : 
- -s : Scansion du texte (-s1 : avec recherche des mètres).
- -a : Accentuation du texte (avec options -a1..-a3 et -a5..-a7).
- -l : Lemmatisation du texte (avec options -l0..-l8).
- -t : Langue cible pour les traductions (par exemple -tfr, -tuk).
- -C : Majuscules pertinentes.
- -c : Majuscules non-pertinentes.
- -? : Affichage de l'aide.


lundi 4 janvier 2016 

<!-- voir syntaxe Algo -->

## COMPILATION
Requis :
- Un compilateur C++ ;
- Les bibliothèques Qt 5.
- Ajuster le PATH de la machine pour que
  compilateur et bibliothèques soient accessibles
  depuis le répertoire de développement ;
- En ligne de commande :
  * $ qmake
  * $ make
- Créer les répertoires bin/, bin/data et bin/data/dicos
- Placer dans bin/data/ les lexiques téléchargeables 
  à l'adresse [http://outils.biblissima.fr/collatinus/](http://outils.biblissima.fr/collatinus/)
- Générer le fichier de traductions anglaises :    
  $ lrelease collatinus.pro    
… et le déplacer dans bin/ :    
  $ mv collatinus\_en.qm bin/
>>>>>>> master
- Placer dans bin/data/dicos les dictionnaires xml et
  djvu les dictionnaires téléchargeables à la même adresse ;
- Générer le fichier de traductions anglaises :    
  $ lrelease collatinus.pro   
  … et le déplacer dans bin/ :    
  $ mv collatinus\_en.qm bin/
- Lancer Collatinus en ligne de commande :    
  * $ cd bin
  * $ ./collatinus    
  ou après avoir installé un raccourci.

## BOGUES ET PROBLÈMES
- assimilation + contraction :
  * adservasti/asservasti
  * adsignasti
  * assimulasti
  * immolarit/inmolarit 
  * inperarit/imperarit
  Solution provisoire avec un blocage de boucle infinie.

- Syntaxe : sujet, verbe : la virgule ne bloque pas le lien.
- Flexion de multus et ses degrés allogènes (plus, plurimus)

## À FAIRE :
- Créer une branche exponct (u exponctué de volvo, sanguinis, unicode 1EE5 ụ )
- Lemme::pos étant un char, le transformer en string, pour tenir compte des pos multiples.
- éventuellement : possibilité de déclarer un lien comme sûr,   
    ce qui permet d'éliminer par le suite des concurrents.
- téléchargement : éliminer les fichiers homonymes d'une 
  version antérieure.
- Fichier en paramètre
- Flexion : revenir en début de page
- Compiler pour Win
- Version web (emscripten ?)
- écrire un man
- Dans le presse-papier, insérer une signature "édité avec C11+url";
  idem pour l'impression ?
- Nettoyage des -pte -met -quoi dans les traductions ;
- chargement des lexiques et dictionnaires, compressés,
  prêts à être décompressés et installés par Collatinus.
- Modèles :
  *	Vérifier les modèles ;
  *	Traiter les noms 3ème decl pl. majores, orum, donné sing. ;
  * modèles nolo et malo ;
  * Vérifier facio et fio ;
  *	Construire des modèles passifs/intransitifs.
- Système d'hyperliens dans les résultats, qui permet d'afficher
  des entrées. Par exemple, le lemme multus donne ses degrés par
  un hyperlien vers plus, pluris, et vers plurimus, a, um.

## Branche syntaxe
   * But : ne donner que le(s) père(s) du mot cliqué, avec traduction.
   * Arbre en arcs :
     Forte potantibus his apud Sextum Tarquinium, ubi et Collatinus cenabat Tarquinius, Egeri filius, incidit de uxoribus mentio.  
     ^     |  ^ | |   ^   ^ ||  ^ |        ^      ^ | ^   | | ^      | ^      ^  |         ^   | ^     | | |  ^|    ^       ^  
     |     |  | | |   |   | ||  | |        |      | | |   | | |      | |      |  |         |   | |     | | |  ||    |       |  
     +----/   | | +---+   | ||  | |        |      | | +---+ | +------+ |      |  |         +---+ |     | | +--/ \---+       |  
              | +---------+ | \-+ +--------+      | |       +----------|------+  +---------------+     | +------------------+  
              |             +---------------------/ \------------------+                               |  
              +----------------------------------------------------------------------------------------+  
   * **Contrainte** : Tout mot doit trouver son sub, sauf un, qui est
      un verbe conjugué à l'indicatif ou à l'impératif, Quelquefois un
	  nom.
   * **Hypothèse** : En allant de G à D, lorsque un mot a trouvé son super, les
     mots suivants ne peuvent être sub de ce mot.
   * **Hypothèse** : les prépositions et conj. de subordination sont bloquantes
      - elles ne permettent aux mots précédent de chercher des sub après elles
	    que si elles-mêmes ont trouvé un sub.
      - Elles n'ont pas d'homonyme, et si elles en ont,
	    ils sont eux aussi bloquants (ex. *cum*)
	    (vraiment pas sûr). Le pr. relatif peut
	    être considéré comme bloquant, mais il a des formes ambiguës.
   * Attention aux hyperbates : _Collatinus cenabat Tarquinius_
    Collatinus cenabat Tarquinius.
       ^   |     |        ^
       |   +-----|--------+
       +---------+    

## Branche maj
Branche de mise à jour des lexiques.
- Créer un dépôt distant,
- Dans le dépôt, des fichiers compressés, et une page d'accueil et des liens ;
- Dans Collatinus, menu Lexique ou Aide, ou les deux, une option màj ;
- Créer le dialogue maj 
  * lien vers le dépôt, mode d'emploi ;
  * Un bouton qui lance un QFileDialog de sélection des fichiers téléchargés ;
  * Un bouton OK pour lancer la décompression et copie des fichiers ;
  * Un bouton Annuler.
- Moyens d'identifier la version des dictionnaires et lexiques installés :
  1. Version dans l'en-tête de chaque fichier ;
  2. Faire marcher C11 avec des fichiers versionnés, et indication de remplacement ;
  3. Paquet versionné, prise de version et enregistrement dans les préférences, 
     et dépaquetage sans version.


## DOC
- Un modèle d'utilisation des docks est dans les exemples Qt5 : 
  ${Doc-qt5}/examples/widgets/mainwindows/dockwidgets
