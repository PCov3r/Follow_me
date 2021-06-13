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




Sources utilisées pour ce projet : <br>

https://www.arducam.com/docs/cameras-for-raspberry-pi/ptz-camera/software/
