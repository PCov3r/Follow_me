# Follow_me

***Follow me*** est un projet de première année d'école d'ingénieur.<br>
Le but est de suivre une personne grâce à un algorithme de reconnaissance faciale et de faire du streaming. On utilise ainsi une raspberry, une carte STM32 et 2 servo-moteurs (un pour le pan et un pour le tilt).<br> On a également implémenté un contrôle manuel du tracking par le biais d'API coté serveur et d'une autre raspberry et d'une STM32 coté commande.



## Serveur et streaming

[Section relative au serveur flask et au tracking](WebServer/FlaskServer/)

On a utilisé la librairie OpenCV pour faire du tracking et la librairie Flask pour implémenter le serveur web et le streaming.<br>

<br>

Liste des packages à installer pour le serveur web et tracking:

pip : ``sudo apt-get install python3-pip`` <br>
imutils : ``pip install imutils`` <br>
opencv : ``apt-get install python3-opencv``<br>
flask : ``pip install flask``


## Commande manuelle

[Section relative à la commande manuelle du tracking](WebServer/clientSide/)

## Contrôle des mouvements de la caméra

[Section relative au contrôle des mouvements de la caméra](/servo/)

### Les servomoteurs (Mouvement pan et tilt)

Afin de contrôler les servomoteurs qui régissent les mouvements de la caméra, nous avons crée une libraire en C pour traduire les instructions en une série de valeurs que les servomoteurs comprennent. Les moteurs ont une mémoire (une RAM qui s'efface à chaque mise hors tensionn et une ROM permanente) dans laquelle nous pouvons stocker des valeurs. Chaque case mémoire contient l'état d'un certain paramètre du moteur : une case stocke l'angle du moteur, une autre stocke sa vitesse et une autre stocke sa température interne par exemple. Donc pour faire tourner le moteur, nous n'envoyons pas "tourne à 120°" mais plutôt "écrit 120° dans la case mémoire qui stocke l'angle du moteur". Les cases mémoires ainsi que leur fonction sont accessibles ici <href="https://www.linguee.fr/francais-anglais/search?source=auto&query=alimentation">.

### Le zoom et le focus de la caméra

Sources utilisées pour ce projet : <br>

https://www.arducam.com/docs/cameras-for-raspberry-pi/ptz-camera/software/


# Du POE !

Notre cahier des charges imposait également la conception d'un shield POE. <br>
L’objectif de notre Shield POE est de recevoir le courant par un câble ethernet branché à la Raspberry Pi 4B afin d’alimenter cette dernière, mais aussi les moteurs permettant d'orienter la caméra. <br>

**Versioning:** <br>
<br>
* V1 <br>
 La tension fournie par le distributeur POE au travers du câble Ethernet est une tension continue comprise entre 42V et 52V, il nous a donc fallu utiliser deux abaisseurs de tensions afin d’obtenir les tensions de 5V et 11.1V nécessaires respectivement pour l’alimentation de la Raspberry Pi 4B et des moteurs. <br>
La première tension souhaitée est créée à partir de l’abaisseur LM2576HVS (utilisé dans la configuration présentée à la page 19 de la DataSheet avec lesvaleurs de composants calculés grâce aux instructions et tables données page 19,20,21). Le montage entourant l’abaisseur 11.1V  LM46002-Q1 est également tiré de sa propre Datasheet page 24. <br>
Les valeurs des composants de ces deux montages retenues apparaissent dans le Schematics du projet EAGLE.

* V2 <br>
 La première version avait pour seul objectif de vérifier que les montages sélectionnés permettaient en effet d’obtenir les tensions désirées. Une fois cela validé, nous avons créé cette deuxième version, comportant les mêmes circuits, mais sur un PCB adapté à la Raspberry Pi 4B. Le shield se fixe directement sur les 4 pins de la Raspberry Pi 4B destinés à la redistribution de l'énergie reçue par le cable ethernet, et sur les pins 2 et 6 permettant directement d’alimenter la Raspberry Pi 4B. 
 
* V3 <br>
 Nous avons réalisé lors de la réalisation de la deuxième version du PCB que l’attribution des pins destinées au POE sur la Raspberry dépendent du câble ethernet et de l’injecteur POE utilisés. Afin que notre shield puisse fonctionner correctement avec tous les câbles ethernet et tous les injecteurs POE, nous avons rajouté sur cette version finale un pont de diodes, qui, peu importe l’attribution des pins de la Raspberry Pi 4B, nous garantit une tension positive et une masse.

**DISCLAIMER:** <br>
La norme POE+ nécessiterait un composant supplémentaire (non implémenté ici) sur le shield pour être utilisable. Ce shield POE n’est donc absolument pas compatible avec la norme POE+, et ne fonctionnera pas si cette norme est sélectionnée dans les paramètres de l’injecteur. (Si vous sélectionnez la norme POE+ avec ce shield POE, l’injecteur délivrera une tension intermittente bien en dessous de la valeur attendue, et sera donc inutilisable.)  Be advised.

