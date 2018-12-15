# Vers Collatinus-12, notes

Je numérote les paragraphes (§n. comme dans le Cart-Grimal) pour pouvoir les citer par la suite (Ph. 22 octobre 2018)

## vendredi 21 septembre 2018 (Y.)

# Format des données optionnelles chargeables et déchargeables.

- §1. Elles sont livrées sous forme de paquet qui se décompresse dans l'espace
  personnel de l'utilisateur ;
- §2. Dans un paquet :
  * lexique
  * traductions
  * particularités graphiques (assimilations, contractions, équivalences graphiques)
  * éventuellment, modèles
- §3. Ces paquets doivent pouvoir être chargés et déchargés sans quitter
  l'application.
- §4. Question : peut-on charger plusieurs paquets à la fois ? A priori, non.
- §5. Les paquets peuvent, au lieu de modifier le tronc commun, modifier un
  autre paquet.
- §6. **Deux possibilités de chargement**
    1. On charge d'abord le tronc commun (ou le paquet père), puis le paquet
      qui modifie le tronc commun.
    2. Le paquet et le tronc commun ont été compilés, et il n'y a plus qu'un jeu
       de données : modèles, lemmes, irreguliers, alternances.
- §7. **Interface** -- à l'initialisation, le répertoire des paquets optionnels est scané,
  et un dialogue propose leur chargement.
- §8. Un paquet doit, en plus des données, fournir une description de sa cible :
  époque / genre / auteur / lieu.
- §9. **Action des données optionnelles sur les données permanentes**    
  Les données permanentes ne sont pas déchargées au chargement des données
  optionnelles, mais elle peuvent être modifiées par elles. Liste des 
  actions des données optionnelles :
  * ajouter un lemme et ses traductions ;
  * supprimer un lemme et ses traductions, apparemment inutilisés, parce
    qu'un auteur est toujours susceptible d'employer un archaïsme. On
    supprimera un lemme :    
    seulement si son apparition est ultérieure au corpus cible,   
    seulement si le lemme à supprimer peut créer des ambiguïtés.
  * modifier seulement la graphie et le modèle d'un lemme ;
  * modifier les traductions d'un lemme, quelquefois en changeant l'ordre
    d'apparition de ces traductions ;
  * supprimer des traductions (sens apparu ultérieurement au corpus visé) ;
  * ajouter des traductions sans supprimer celles des données permanentes ;
  * même principe pour les autres données : modèles, assimilations,
    contractions, équivalences.
- §10. **Syntaxe**
  * en-tête : Dans l'interface de C12, les analyses et traductions issues
    d'un paquet optionnel doivent être identifiables.
  * Il faudra trouver le moyen d'indiquer, dans les données, ce qu'il faut
    ajouter et supprimer, (supprimer et ajouter au même endroit = remplacer)
  * un fichier de paquet optionnel est partagé en plusieurs sections :
    - lignes à ajouter
    - lignes à compléter
    - lignes à supprimer
  * Exemple : lemmes.fr donne    
   miles:soldat    
   le paquet med (= médiéval) aura, dans la partie "lignes à compléter" :    
   miles:chevalier    
   L'interpréteur de paquets ajoutera donc "class." devant la partie tronc commun,
   et "med.", càd le nom du paquet, devant l'ajout, ce qui donnera dans l'interface :
   miles, itis, m. : class. soldat med. chevalier
- §11. **Problème du classement des traductions** -- Les dictionnaires papier
  suivent plutôt un ordre chronologique. Par exemple, /lego/ a comme
  premier sens « cueillir ». Faudrait-il adopter un ordre fréquenciel ? On
  peut aussi, garder l'ordre chronologique, et trouver le moyen d'indiquer
  les fréquences.

## Le 22 octobre 2018 (Ph.)

J'ai ajouté des arguments supplémentaires pour la modularité des données dans un futur C12.

# Dépouillements de dicos

- §12. Motivés par la publication de [LemLat3] (www.lemlat3.eu) d'un lexique étendu vers le moyen-âge grâce au Du Cange, nous envisageons d'augmenter aussi le lexique de Collatinus. Dans l'annonce du 3 septembre, Marco Passarotti annonçait 82,556 lemmes. Étrangement, si je fais du dénombrement sur le fichier de l'ENC, je trouve 90 406 formes principales (*class="form b">*) et 15 481 formes secondaires (*class="form sc">*). On ne serait pas loin de 106 000 lemmes. Peut-être avec quelques redondances. Toutefois, il y a une différence majeure (et un obstacle) : Collatinus doit proposer des traductions. Le Du Cange n'en propose pas beaucoup.
- §13. Dans le cadre de l'atelier *lemmes* de Cosme<sup>2</sup>, il a beaucoup été question d'entités nommées et il pourrait être judicieux d'aborder les chartes et autres textes médiévaux avec une besace de toponymes et de patronymes. À partir du [dictionnaire topographique de France] (http://cths.fr/dico-topo/index.php), j'ai constitué un pré-lexique de plus de 60 000 noms qui semblent latins. Je n'ai pas encore d'idées précises sur le nombre de patronymes que l'on pourrait réunir.

§14. Un tel accroissement du volume de données risque de saturer rapidement les capacités de mémoire de l'ordinateur sur lequel tourne Collatinus.

# Commentaires

- §15. Avant de discuter le format des données optionnelles, ne faudrait-il pas choisir un principe de fonctionnement de C12 ? Le §10 (syntaxe des paquets optionnels) me semble ainsi un peu prématuré. Bien sûr, le problème forme un tout. Mais les formats et syntaxes sont plutôt des conséquences de ce que l'on souhaite (et pourra) faire.
- §16. Je suis entièrement d'accord avec le fait que l'utilisateur doit pouvoir disposer d'un paquet qui se décompresse là où il faut (§1). Également d'accord avec le §3, que l'utilisateur doit pouvoir choisir quelle(s) extension(s) il veut utiliser sans avoir à quitter et relancer le programme. L'interface doit proposer le choix des paquets au démarrage (§7) et aussi à tout moment en passant par le menu adhoc.
- §17. Je suis plutôt en désaccord avec l'avis exprimé au §4 (ne pas charger plusieurs paquets), d'ailleurs en contradiction avec le §5. Considérant les extensions possibles du lexique (cf. §12 et §13), je me vois mal utilisant les toponymes, mais pas les patronymes ou inversement.
- §18. Savoir si ce paquet reste un seul fichier ou s'il se disperse en un certain nombre de fichiers peut se discuter. Personnellement, je serais plutôt favorable à la deuxième solution qui correspond à la description du §2.
- §19. Un paquet a-t-il une cible ou une source (§8) ? La vision que j'en ai (§12 et 13) me fait pencher plutôt pour la source. Si on réunit le "vocabulaire de Cicéron" dans un paquet, on peut penser qu'il va viser (permettre de lemmatiser et de comprendre) la prose de Cicéron. Mais je pense qu'il faut plutôt souligner son origine qui est l'ensemble des textes de Cicéron que nous avons conservés ou dépouillés.

# Variantes graphiques et extensions de vocabulaire

§20. Dans un des mails échangés, Yves s'étonnait que je sépare le traitement des variantes médiévales indépendamment et séparément des extensions. Effectivement, dans mon esprit, il s'agit de deux choses différentes qui méritent donc des traitements différents. Pour donner un exemple, je peux imaginer un texte médiéval édité avec des graphies classiques mais avec un vocabulaire irréductiblement médiéval. L'inverse, un texte avec des graphies médiévales n'employant que du vocabulaire classique, est-elle possible ? Pourquoi pas. Il est toutefois vrai que l'emploi simultané des deux aspects est hautement probable. Mais aussi que qui peut le plus peut le moins.

# Recherches sur fichiers vs tout en mémoire.

§21. Pour faire face à l'augmentation du volume de données, j'avais proposé de conserver les principes de C11 mais avec un changement radical qui serait que plus rien n'est en mémoire vive : tout reste dans des fichiers. Yves a opposé à cela que la force et la rapidité de Collatinus viennent de ce que tout est en RAM. Peut-être bien. Mais entre rien et tout en mémoire vive, il y a sans doute un juste milieu. On pourrait imaginer que le lexique de base soit entièrement en mémoire vive et que les extensions restent sur fichier. Mais cela introduit une dissymétrie et cela suppose que si on a trouvé une analyse dans le lexique de base on ne va pas en chercher d'autres dans les extensions. Des homographes accidentels ne seraient plus alors identifiés. Ainsi, la lemmatisation de *oris* ne donneraient pas les très improbables *orum* (pour *aurum*), *oriae* (pour *oreae*) ou *orion*.

§22. Une autre solution qui me semble très prometteuse consiste à garder en mémoire les désinences et les radicaux mais associés à une information très réduite : la position dans le fichier où aller chercher toute l'information. Peut-être que pour les désinences, que l'on utilise très souvent, le chargement en mémoire est justifié. Toujours est-il que l'on doit faire une économie substantielle de place : on associe au radical un simple entier sur huit octets. Je ne sais pas exactement quelle place occupe un Radical dans C11, mais il est composé de deux chaines (QString, donc deux octets par caractères), d'un pointeur et d'un entier : ça doit faire une quarantaine d'octets. Si les objets en mémoire occupent deux fois moins de place, on peut en mettre deux fois plus sans risque.

§23. Cela dit, on met peut-être la charrue avant les bœufs : qui nous dit que l'augmentation du volumes des données va poser un problème à C11. Avant de justifier la création d'un C12 par cet obstacle hypothétique, ne faudrait-il pas essayer de charger dans C11 des extensions comme le Du Cange, des toponymes et des patronymes ? Rien que pour voir. Il faudrait bien sûr créer ces ressources avant de pouvoir faire des essais avec...

## Le 24 octobre 2018

# Plan de travail (Yves)

- §24. Transférer entièrement dans les données tout ce qui permet à
  Collatinus de lemmatiser le médiéval, essentiellement le traitement
  des graphies. Plus généralement, mettre autant que possible en data
  ce qui est codé en dur ;
- §25. Faire un premier essai d'extension, côté créateur :
  * choisir un corpus caractéristique, réduit de préférence ;
  * copier data/ dans un répertoire class/ (classique)
  * faire dans data/ toutes les modifications nécessaires pour que
    Collatinus lemmatise correctement le corpus ;
  * ajouter un petit fichier de description de l'extension ;
  * diff class/ data/ > ext_test.diff (compressé si l'on veut);
  * placer ce diff dans un répertoire ext/ ;
  * rétablir l'état initial en copiant class/ dans data/
- §26. côté utilisateur :
  * créer un menu extensions (ou autre) qui balaіe ext/ et propose
    les extensions, avec la brève description de chacune ;
  * le choix d'une extension permet de faire le chemin du créateur
    en sens inverse
    - copier class/ dans data/
    - patcher data/ avec l'extension choisie :
      patch data/ ext_test.diff
  * suppression de toute extension :
    - copier class/ dans data/
    
§27. Les données classiques, que nous appelons à tort "tronc commun" sont
donc maintenant dans class/. Pour un simple utilisateur, class/ et
data/ restent semblables. On peut même imaginer que class/ n'existe
pas tant qu'on n'est pas passé aux extensions. Avant l'installation de
la première extension, une copie de data/ est créée dans class/.
Il faudrait aussi que dicos/ quitte data/, et remonte d'un niveau.
En terme d'espace, ce n'est pas horrible : non compressées, les
données occupent 14 M.

§28. Arbo d'un C12 avec extensions :

--+ exécutable
  + doc/
  + data/  (données effectivement utilisées)
  + class/ (données de l'actuel C11)
  ٍ+ ext/   (collection de fichiers diff)
  + dicos/

à l'installation, sans extensions :

--+ exécutable
  + doc/
  + data/
  + dicos/

Ce n'est qu'un premier jet. Nous avons le temps, mais si nous pouvons
présenter quelque chose dans ce style aux réunions de 2019, ce serait
bien. 

## Le 28 octobre 2018

# Commentaires (Ph.)

- §29. Je crois que tout ce qui est médiéval est dans le dossier data (§24.). Les transformations des graphies médiévales sont dans un fichier éditable. La seule chose codée en dur aujourd'hui est le booléen _med. Si j'ai introduit ce booléen, c'est précisément pour que l'utilisateur puisse choisir dans quel mode il souhaite travailler. À terme, ce booléen devra être lié à un item de menu (Lexique ou Extra) et sera sauvé dans les paramètres.

- §30. Côté créateur (§25.), j'essaierais plutôt de créer des extensions au format C11 pour les toponymes, les patronymes et le Du Cange. Ça me semble déjà un vaste programme, surtout si on veut des traductions pour le Du Cange. Un option plus modeste de création d'extension serait d'extraire les ajouts de Frère Romain de lem_ext. On aurait ainsi une première extension, lem_Vulg, avec laquelle faire des essais dans C11 (cela dit, il n'y aurait que ~400 entrées). Pour l'utilisateur (§26.), je ferais évoluer C11 pour que l'on puisse choisir plusieurs extensions et pas seulement **l'extension**, i.e. lem_ext.

