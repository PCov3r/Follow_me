# Follow me

***Follow me*** est un projet de première année d'école d'ingénieur créé par 10 étudiants.<br>
Le but est de suivre une personne grâce à un algorithme de reconnaissance faciale et de faire du streaming. On utilise ainsi une raspberry, une carte STM32 et 2 servo-moteurs (un pour le pan et un pour le tilt).<br> On a également implémenté un contrôle manuel du tracking par le biais d'API coté serveur et d'une autre raspberry et d'une STM32 coté commande.


## Serveur et streaming

[Section relative au serveur flask et au tracking](WebServer/FlaskServer/)

On a utilisé la librairie OpenCV pour faire du tracking et la librairie Flask pour implémenter le serveur web et le streaming.<br>

### Tracking et API

La librairie OpenCV possède de manière native différents tracker, dont les détails peuvent être trouvés [ici](https://learnopencv.com/object-tracking-using-opencv-cpp-python/) et [ici](https://ehsangazar.com/object-tracking-with-opencv-fd18ccdd7369). Dans notre cas, nous utilisons le tracker KCF qui est non seulement précis, mais qui retrouve également la personne si celle-ci sort et rentre dans le champ de la caméra. <br>
Le passage du mode tracking automatique au mode contrôle manuel est assuré par le biais d'[API](https://flask.palletsprojects.com/en/2.0.x/api/) sous forme de requêtes HTTP. Le serveur Flask reçoit ces requêtes, en analyse les arguments, et agit en conséquence.
<br>
### Communication avec la carte STM32

La communication entre la Raspberry Pi et la carte STM32 gérant la caméra et les servo-moteurs se fait via le protocole [UART](https://www.raspberrypi.org/documentation/configuration/uart.md). Des détails sur le protocole mis en place se trouvent dans la section relative au contrôle des servos.
Il est à noter que le contrôle des mouvements est pour le moment très naïf. En effet, on indique simplement à la STM32 dans quelle direction on souhaite orienter la caméra. 
<br>
Nous avons en effet abandonné l'idée d'indiquer précisément à la carte la position relative de l'utilisateur, ceci pour des raisons de simplicité et surtout de temps.
Une source d'amélioration serait ainsi d'implémenter cela et d'agir en conséquence sur la vitesse de déplacement afin de s'assurer de ne jamais perdre l'utilisateur.
<br>
### Liste des packages à installer pour le serveur web et tracking

pip : ``sudo apt-get install python3-pip`` <br>
imutils : ``pip3 install imutils`` <br>
opencv : ``apt-get install python3-opencv``<br>
flask : ``pip3 install flask``

<br>
<br>

## Contrôle des mouvements de la caméra

[Section relative au contrôle des mouvements de la caméra](/servo/)

### Les servomoteurs (Mouvement pan (axe X) et tilt (axe Y))

Afin de contrôler les servomoteurs qui régissent les mouvements de la caméra, nous avons crée une libraire en C pour traduire les instructions en une série de valeurs que les servomoteurs comprennent.<br>
Les moteurs ont une mémoire (une RAM qui s'efface à chaque mise hors tension et une ROM permanente) dans laquelle nous pouvons stocker des valeurs. Chaque case mémoire contient l'état d'un certain paramètre du moteur : une case stocke l'angle du moteur, une autre stocke sa vitesse et une autre stocke sa température interne par exemple.Donc pour faire tourner le moteur, nous n'envoyons pas "tourne à 120°" mais plutôt "écrit 120° dans la case mémoire qui stocke l'angle du moteur".
<p align="center"><a href="https://emanual.robotis.com/docs/en/dxl/ax/ax-12a/#control-table-of-eeprom-area">Les cases mémoires ainsi que leur fonction sont accessibles ici</a></p>
La libraire permet de faire cette traduction pour facilement commander les moteurs. Nous avons codé presque toutes les fonctions présentes sur le site, pour pouvoir réutiliser cette librairie pour d'eventuels futurs projets.<br>
Maintenant que nous savons <i>quoi</i> envoyer, il faut se demander <i>comment</i>. En effet, la communication avec le moteur ne se fait via qu'un seul fil. Le protocole est donc plutôt simple : <br>
- Tant qu'il ne se passe rien, la sortie de la carte reste à l'état haut (5V).<br>
- Si la carte envoie une instruction, alors la sortie va passer à l'état bas (0V), et à partir de là le moteur va commencer à lire la série de 0 et de 1.<br>
- Le moteur interprète la série binaire, exécute l'action correspondante, puis renvoie à la carte une autre série binaire qui peut être une erreur, la valeur se la case mémoire ou bien un code de bon déroulement.<br>
Ce protocole s'appelle l'Half-Uart (en opposition avec l'Uart qui utilise 2 fils : un pour l'envoi et un pour la réception). <a href="https://emanual.robotis.com/docs/en/dxl/protocol1/">Plus de détails sur la communication avec les moteurs ici</a>.

![image](https://user-images.githubusercontent.com/38764918/121864870-a4e20480-ccfd-11eb-8929-7ded6d787731.png)

### Le zoom et le focus de la caméra

Le contrôle du zoom et de la caméra fonctionne exactement comme les moteurs, seul le protocole de comunication change, cette fois-ci c'est l'i2c. Les cases mémoires ainsi que les valeurs [sont décrites ici](https://www.arducam.com/docs/cameras-for-raspberry-pi/ptz-camera/software/). On ajoute aussi la [datasheet de la caméra](https://www.arducam.com/downloads/modules/RaspberryPi_camera/OV5647DS.pdf)
![image](https://user-images.githubusercontent.com/38764918/121865004-c511c380-ccfd-11eb-89d0-1f9671bb432f.png)

### La communication STM/Raspberry

Enfin, c'est la Raspberry qui s'occupe du tracking du visage, c'est donc elle qui envoie les ordres de bouger la caméra. Or, les moteurs sont commandés par la carte STM. Il faut donc établir une communication entre les deux. Nous avons donc établi un code entre nous pour échanger ces instructions, via le protocole Uart.
![image](https://user-images.githubusercontent.com/38764918/121821122-2ce0f380-cc97-11eb-99a7-2016dbee81f1.png)


## Commande manuelle

[Section relative à la commande manuelle du tracking](WebServer/clientSide/)

En plus du tracking, nous avons ajouté à l'utilisateur la possibilité de prendre la main sur le contrôle de la caméra via un boitier de commande relié au serveur principal par API.<br>
Ainsi, grâce à un simple interrupteur, on passe du mode automatique au mode manuel et inversement.
Le contrôle des mouvements selon les axes x et y ainsi que du zoom sont assurés par 2 joysticks. <br> <br>
L'acquisition se fait via une carte STM32, qui communique ces informations à une raspberry via la librairie ``pyserial``. Cette dernière envoie ensuite des requêtes HTTP au serveur Flask via la librairie ``pycurl``.
<br>
De plus amples informations peuvent être trouvées dans les commentaires accompagnant chaque code.

### Liste des packages à installer pour le contrôle manuel de la caméra

pip : ``sudo apt-get install python3-pip`` <br>
pyserial : ``pip3 install pyserial`` <br>
pycurl : ``pip3 install pycurl``

<br>
<br>

## Du POE !
[Section relative au shield POE](/POE/)

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
 
![200465168_157028079806266_8768297953444945326_n(1)](https://user-images.githubusercontent.com/38764918/121863878-96471d80-ccfc-11eb-817b-e354a1085579.png)

**DISCLAIMER:** <br>
La norme POE+ nécessiterait un composant supplémentaire (non implémenté ici) sur le shield pour être utilisable. Ce shield POE n’est donc absolument pas compatible avec la norme POE+, et ne fonctionnera pas si cette norme est sélectionnée dans les paramètres de l’injecteur. (Si vous sélectionnez la norme POE+ avec ce shield POE, l’injecteur délivrera une tension intermittente bien en dessous de la valeur attendue, et sera donc inutilisable.)  Be advised. <br>
La capacité Cin doit selon la datasheet supporter une tension de 100V. Cette dernière est difficile à trouver mais peut être achetée ici: https://fr.farnell.com/tdk/ckg57nx7r2a106m500jh/condensateur-10uf-100v-mlcc-2220/dp/1886985

