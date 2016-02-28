# Collatinus-11, notes

lundi 4 janvier 2016 

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
  [à l'adresse](http://outils.biblissima.fr/collatinus/)
- Placer dans bin/data/dicos les dictionnaires xml et
  djvu les dictionnaires téléchargeables à la même adresse ;
- Lancer Collatinus en ligne de commande :    
  * $ cd bin
  * $ ./collatinus    
  ou après avoir installé un raccourci.

## BOGUES ET PROBLÈMES
- Flexion de multus et ses degrés allogènes (plus, plurimus)

## À FAIRE :
- Créer une branche exponct (u exponctué de volvo, sanguinis, unicode 1EE5 ụ )
- Tester des liens syntaxiques
  * dock syntaxe
  * clic sur un mot -> recherche de liens (as|des)cendants
  * morpho du mot
  * lien avec un autre mot
  * id du lien
  * traduction approximative du lien
  * éventuellement : possibilité de déclarer un lien comme sûr,   
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
