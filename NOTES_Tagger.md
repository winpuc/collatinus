# Collatinus-11, notes sur le Tagger.

Samedi 19 novembre 2016

## Introduction
Le problème de la désambiguïsation est un problème récurrent.
Pour essayer de retrouver à quel lemme associer une forme,
on va utiliser des étiquettes (tags) et tenir compte du contexte
par ces seules étiquettes. Plus précisément, dans une phase d'apprentissage,
on a relevé le nombre de séquences de trois tags. Par la suite,
on s'appuie sur ces statistiques pour estimer la séquence la plus probable.

En l'occurrence, l'apprentissage s'est fait sur les textes lemmatisés
au LASLA. Je remercie Dominique Longrée et toute l'équipe du LASLA 
de m'avoir confié leurs textes et permis de les dépouiller à cette fin.
[Les textes traités](http://web.philo.ulg.ac.be/lasla/textes-latins-traites/)
couvrent l'ensemble du Latin classique.

## Travaux préliminaires

Les textes du LASLA associent une forme à un lemme et à un *code en 9*.
Malheureusement, ces *codes en 9* sont trop nombreux et ne peuvent pas
être utilisés comme tags. En effet, le choix de l'ensemble des étiquettes
(tagset) est un problème délicat. Trop simplifié, il ne permet pas de 
résoudre les ambiguïtés. Trop compliqué, il nécessiterait un corpus 
d'apprentissage gigantesque pour que les statistiques soient représentatives.
Dans un premier temps, j'avais gardé la catégorie donnée par le *code en 9*
ainsi que les cas et nombre pour les formes déclinées ou 
le mode pour les formes conjuguées. Pour ces dernières, il m'a
semblé astucieux de distinguer le présent des autres temps. 
Il aurait pu être utile d'avoir aussi les genres, 
mais le LASLA ne les donne pas. Cela conduit à 178 tags.

Pour passer dans Collatinus, il m'a semblé plus simple de
remplacer la catégorie du *code en 9* par le POS tel que
Collatinus le connaît. On perd ainsi les sous-catégories
définies pour les pronoms (E-L) et les adverbes (M-Q).
J'ai alors 91 tags toujours codés sur trois caractères.
Un tag spécial, snt, est associé à la fin de phrase.

Le premier caractère est le POS.
* a : adjectif
* c : conjonction
* d : adverbe
* i : interjection
* m : nombres
* n : nom
* p : pronom
* r : préposition
* v : verbe (forme conjuguée)
* w : forme verbale déclinée

Pour les formes déclinées (a, n, p et w), le deuxième
caractère est le cas (de 1 à 6 du nominatif à l'ablatif, 
dans l'ordre en usage en France, et 7 pour le locatif)
et le troisième est le nombre (1 ou 2).

Pour les formes verbales conjuguées, le deuxième caractère
est le mode.
* 1 : indicatif
* 2 : subjonctif
* 3 : impératif
* 4 : infinitif
Le troisième caractère sert à distinguer le présent des autres temps.
Il vaut 1 si c'est un présent. Sinon, c'est un espace.

Les supins, trop peu nombreux, ont été mélangés avec les impératifs
non-présents (v3 ). Il traine aussi des tags hérités du LASLA, qui ne sont
pas utilisés par Collatinus. En particulier, le # pour l'auxiliaire être.

Avec ces 91 tags, j'ai aussi relevé les trigrammes (séquences de 3 tags
séparés par un espace, donc sur 11 caractères). 
Il y en a 114 981.
Sont aussi dénombrés les bigrammes de début ou de fin de phrase 
(chaines de 7 caractères qui commencent ou finissent par "snt").

Le dépouillement des fichiers du LASLA m'a également permis
d'établir des liens entre les lemmes utilisés au LASLA et ceux
de Collatinus. Je peux donc attribuer aux entrées du lexique
de Collatinus le nombre d'occurrences trouvées dans les textes.
Le gros du travail a été fait par un petit programme.
Il reste encore des cas difficiles à résoudre. Les cas simples
(pouvant être réglés par un non-latiniste) ont été passés en revue.

Les résultats de ces dépouillements sont dans les fichiers
tags.la et nombres.la. Le format de ces fichiers est décrit
au début de chacun d'eux. Il s'agit toujours de texte simple,
donc facile à lire et à réutiliser. Ici, les champs sont séparés
par une virgule (format CSV).

## Ordonner les lemmatisations

Jusqu'à présent, l'ordre dans lequel Collatinus présentait
ses résultats de lemmatisation n'était lié qu'à l'ordre dans
lequel il les avait trouvés. Maintenant que je connais le
nombre d'occurrences de chaque lemme dans les textes du LASLA,
je peux trier ces solutions pour que la plus fréquente soit
proposée en premier. Toutefois, Collatinus n'est pas un
lemmatiseur de formes (c'est à dire qu'il ne travaille pas
avec la liste des formes possibles associées à un nombre
d'occurrences). Il doit donc *estimer* le nombre d'occurrences
d'une forme à partir de l'analyse morphologique et du nombre
d'occurrences du lemme associé.

J'utilise donc une partie des résultats extraits du dépouillement
des textes du LASLA :
* le nombre d'occurrences des lemmes
* le nombre d'occurrences des tags

Ici, je n'ai pas besoin des séquences de tags puisque je
considère un mot hors de tout contexte.
Je fais implicitement une hypothèse assez forte qui consiste
à supposer que tous les lemmes sont employés à tous les cas et nombres
de la même façon (en tenant compte de la catégorie du mot, évidemment).
Quand je veux associer une forme à un lemme avec un tag spécifique,
je suppose que le nombre d'occurrences à considérer est
simplement le produit du nombre d'occurrences du lemmes et de la
proportion du tag dans la famille. Il faut bien voir que ce n'est là
qu'une approximation et que je ne prétends pas donner une estimation fiable.
Un lemmatiseur de forme donnerait lui le nombre d'occurrences observées.
Exactement.

## Principe du tagger

Un tagger probabiliste va construire, avec les formes qui sont dans
la phrase, toutes les séquences de tags possibles et
leur associer une probabilité. La séquence retenue sera
celle qui a la plus forte probabilité.
La phrase est traitée mot après mot dans l'ordre naturel.
J'ai pris soin de conserver une probabilité, c'est à dire
que si on faisait la somme de toutes les probabilités associées
à chacune des séquences, on trouverait 1.

En pratique, il n'est pas possible de conserver toutes les séquences
car leur nombre croît exponentiellement avec la longueur de la phrase.
Heureusement, le fait de ne considérer que des trigrammes permet
d'introduire un mécanisme d'élagage très efficace. On va donc
progresser en deux temps :
* pousse de toutes les nouvelles branches
* élagage des branches stériles.

La représentation en arbre de l'ensemble des séquences possibles
est assez tentante et naturelle. Toutefois, le mécanisme d'élagage
décrit plus bas s'apparente à du repiquage dans le sens où les
nouvelles branches poussent sur des "troncs" différents.
J'ai donc préféré utiliser une liste de toutes les séquences utiles.

### Nouvelles pousses

Lorsque l'on a déjà traité n-1 mots de la phrase, on a associé à 
chacune des séquences obtenues avec les tags de ces n-1 mots
une probabilité. À chacune de ces séquences, on va ajouter tous 
les tags possibles pour le énième mot et calculer les nouvelles
probabilités. Comme pour la lemmatisation, on a une estimation
du nombre d'occurrences de la forme associée à chacun des tags.
On connaît aussi le nombre observé des trigrammes obtenus en
complétant le bigramme terminal d'une séquence pour n-1 mots
par le tag choisi pour le énième mot. Pour chaque bigramme
terminal, on va construire une probabilité conditionnelle
à partir de ces produits de nombres d'occurrences. Il s'agit 
donc de ces produits divisés par leur somme.

La probabilité d'une séquence obtenue en complétant une de 
celles de n-1 mots par un tag du énième mot est simplement
le produit de la probabilité associée à la séquence de départ
et de la probabilité conditionnelle évaluée plus haut.
On vérifie aisément que c'est une probabilité : la somme
de tous les possibles est toujours 1.

### Élagage

Le fait que l'on considère le contexte uniquement à travers
les trigrammes et le calcul des probabilités présenté plus haut
montrent qu'il est inutile de conserver deux séquences qui se 
terminent par le même bigramme. En effet, supposons que nous 
ayons deux telles séquences associées à des probabilités P1 et P2.
On supposera que P1 > P2. Puisque j'ai le même bigramme terminal,
les probabilités conditionnelles associées à l'ajout du énième mot
sont les mêmes. Toutes les séquences issues de la deuxième séquence
considérée auront des probabilités plus petites que le séquence
correspondante issue de la première. Les ajouts successifs de mots
ne permettront jamais à une séquence issue de la deuxième de 
surpasser la meilleure de celle issue de la première.

Pour *chaque bigramme terminal*, il faut conserver la séquence
qui est associée à la plus grande probabilité. C'est la seule
qui puisse conduire à la séquence finale 
(avec tous les mots de la phrase) la plus probable.

### Information sur les autres tags

On a vu qu'après la phase de pousse, on ne conserve qu'une
séquence pour chacun des bigrammes terminaux possibles.
On risque alors de faire disparaître certains des tags
(les moins favorables) pour le mot en position n-2.
En particulier, on ne saura jamais avec le seul résultat
final (la séquence la plus probable) si on n'a pas laissé
en route une autre possibilité à peine moins probable.
J'ai donc décidé de conserver, avant de procéder à l'élagage,
la meilleure probabilité associée à chacun des tags du 
mot n-2 lorsque j'ai ajouté le énième mot.

Je dois faire cette sélection avant l'élagage, car c'est
le dernier moment où je suis sûr que tous les tags possibles
du mot n-2 sont encore présents dans la liste des séquences.
