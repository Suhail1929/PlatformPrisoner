# PlatformPrisoner

I.	Présentation du jeu

Le but du jeu est de survivre et de s'échapper d'un labyrinthe rempli de pièges mortels en étant le premier joueur à atteindre la sortie. Le scénario du jeu se déroule dans un futur lointain, où les joueurs incarnent des prisonniers qui ont été capturés par une race extra-terrestre.
Les joueurs contrôlent un personnage et naviguent à travers des tableaux (des pièces) en récupérant des clés pour ouvrir des portes et éviter les pièges. Le jeu est compétitif, chaque joueur doit être le premier à atteindre la sortie avant les autres joueurs.
Le jeu offre une expérience de jeu en ligne excitante et permet aux joueurs de s'affronter dans des compétitions palpitantes. Les joueurs peuvent également interagir les uns avec les autres pour coopérer ou entraver leur progression.
En somme, votre jeu de plateformes multi-joueurs en réseau offre un mélange captivant de compétition, d'action et de coopération, avec un scénario intéressant qui immergera les joueurs dans un univers futuriste et extraterrestre.

II.	Prérequis

Pour pouvoir lancer le jeu, plusieurs prérequis techniques sont nécessaires. Tout d'abord, il est essentiel de disposer d'un compilateur C pour pouvoir compiler le code source du jeu. Il peut s'agir, par exemple, de GCC, Clang, ou tout autre compilateur compatible avec le langage C.
Ensuite, il est important d'avoir installé le logiciel Make, qui permet de générer automatiquement l'exécutable du jeu à partir du code source. Make est un outil puissant pour automatiser le processus de compilation et faciliter le développement de projets logiciels.
Enfin, le jeu utilise une interface utilisateur basée sur la bibliothèque NCurses. Il est donc nécessaire d'installer cette bibliothèque pour pouvoir afficher les éléments graphiques du jeu à l'écran. NCurses est une bibliothèque de programmation qui permet de créer des interfaces utilisateur en mode texte, avec des fonctionnalités telles que la couleur, la saisie au clavier, etc.

III.	Lancez le jeu !
Dans le cadre de notre projet, il est essentiel de savoir comment lancer l'éditeur de carte et le serveur/client pour commencer à jouer. Tout d'abord, pour lancer l'éditeur, il faut se positionner dans le dossier approprié et taper "make" dans la console. 
../PlatformPrisoner$   make

Ensuite, il suffit de se rendre dans le dossier "bin" pour lancer l'éditeur en utilisant la commande "<nom_fichier.bin>". Ce dernier permet de créer des cartes pour le monde de jeu.
../PlatformPrisoner$ ./bin/editor <nom_monde>

Une fois que la carte est créée, il est temps de lancer le serveur et le client. Pour ce faire, les utilisateurs doivent exécuter les fichiers respectifs pour chaque composant. Le serveur est chargé de gérer les connexions et les interactions entre les joueurs, tandis que le client permet aux joueurs de se connecter au monde et d'interagir avec lui. Il est important de s'assurer que les fichiers sont correctement configurés avant de les exécuter.
../PlatformPrisoner/bin$ ./bin/client <ip > <port>

Enfin, pour commencer à jouer, les joueurs doivent se connecter au monde en utilisant le client. Une fois connectés, ils peuvent explorer le monde, interagir avec les autres joueurs. 
