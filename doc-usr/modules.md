*Collatinus 12 -- guide*

[index](index.html) précédent : [Lemmatiser](lemmatiser.html) suivant : [Dictionnaires](dictionnaires.html) 

Modules
=======

Les données lexicales et morphologiques de Collatinus sont enregistrées
dans des fichiers au format texte. Pour le lexique :

- lemmes.la pour les mots latins ;
- et lemmes.fr, lemmes.it, lemmes.es, etc., pour les langues cibles.

pour la morphologie :

- modeles.la, qui donne les radicaux et désinences de chaque modèle ;
- morphos.fr, it, es, etc, qui donnent toutes les morphologies dans
  les langues cibles.

Il faut ajouter le fichier vargraph.la, qui donne toutes les variantes
graphiques de la langue latine. Pour le lexique classique, ce fichier
est vide.

Ce jeu de fichiers permet de lemmatiser le corpus classique avec un bon taux
de réussite, mais il est moins efficace lorsqu'on aborde le corpus
médiéval, et même le latin de Plaute ou de Térence.

Un module est en fait un ensemble du même jeu de fichiers qui connaît les
mots utilisés par le corpus, leur traduction, ainsi que ses particularités
graphiques et morphologiques. Lorsqu'un module est installé et qu'on veut
l'utiliser, Il est chargé en premier lors de l'initialisation de
Collatinus. Les données classiques sont ensuite chargées, mais en cas de
doublon entre les données du module et les données classiques, priorité est
donnée au module.

Deux premiers modules ont été créés, et sont téléchargeables à partir du site
[biblissima.fr](https://outils.biblissima.fr/fr/collatinus/), sous la forme de
paquets. Un paquet est en fait une archive qui compresse en un seul
fichiers tout les fichiers du module. Les deux premiers paquets ѕont
- hagio.col, qui permet de lemmatiser le [corpus des hagiographies
  bourguignones du Moyen-Âge](http://www.cbma-project.eu/%C3%A9ditions/textes-hagiographiques.html) ;
- plaute.col, qui permet de lemmatiser l'ensemble des comédies de Plaute.

Une fois le paquet téléchargé, le menu _Lexique/Modules lexicaux/Installer
un module_ lance automatiquement la décompression et l'installation des
fichiers du module. Il faudra ensuite se rendre dans _Lexique/modules
lexicaux/activer, désactiver, gérer les modules_, sélectionner le module
qu'on vient d'installer, et cliquer sur le bouton _activer_.

Si le module adapté au corpus qu'on veut lemmatiser n'existe pas, il est
possible d'utiliser un éditeur spécialisé, **Ecce**, qui permet de se
servir des modules existants et de l'extension pour créer un nouveau
module.
