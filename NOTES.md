# Collatinus-11, notes

lundi 4 janvier 2016 

## COMPILATION
Requis :
- Un compilateur C++ ;
- Les bibliothèques Qt 5.\*
- Ajuster le PATH de la machine pour que
  compilateur et bibliothèques soit accessibles
  depuis le répertoire de développement ;
- En ligne de commande :
  * $ qmake
  * $ make
- Créer les répertoires bin/, bin/data et bin/data/dicos
- Placer dans bin/data les lexiques téléchargeables 
  [à l'adresse](http://outils.biblissima.fr/collatinus/)
- Placer dans bin/data/dicos les dictionnaires xml et
  djvu les dictionnaires téléchargeables à la même adresse ;
- Lancer Collatinus en ligne de commande :    
  * $ cd bin
  * $ ./collatinus    
  ou après avoir installé un raccourci.

## BOGUES
- Ligne de saisie non connectée dans certains docks.

## À FAIRE :
- Dans le copier-coller, insérer une signature "édité avec C11+url";
- Nettoyage des -pte -met -quoi dans les traductions ;
- chargement des lexiques et dictionnaires, compressés,
  prêts à être décompressés et installés par Collatinus.
- Préparer la traduction anglaise de l'interface ;
- Modèles :
  *	Vérifier les modèles ;
  *	Traiter les noms 3ème decl pl. majores, orum, donné sing. ;
  * modèles nolo et malo ;
  * Vérifier facio et fio ;
  *	Construire des modèles passifs/intransitifs.

## DOC

Un modèle d'utilisation des docks est dans les exemples Qt5 : 
${Doc-qt5}/examples/widgets/mainwindows/dockwidgets
