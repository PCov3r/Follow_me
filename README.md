# Follow_me

***Follow me*** est un projet de première année d'école d'ingénieur.<br>
Le but est de suivre une personne grâce à un algorithme de reconnaissance faciale et de faire du streaming. On utilise ainsi une raspberry, une carte STM32 et 2 servo-moteurs (un pour le pan et un pour le tilt).<br> On a également implémenté un contrôle manuel du tracking par le biais d'une autre raspberry et d'une STM32.



## Côté serveur

[Section relative au côté serveur](WebServer/FlaskServer/)

Liste des packages à installer pour le serveur web :

pip : ``sudo apt-get install python3-pip`` <br>
imutils : ``pip install imutils`` <br>
opencv : ``apt-get install python3-opencv``<br>
flask : ``pip install flask``


## Côté client

[Section relative au côté serveur](WebServer/clientSide/)

## Côté serveur

[Section relative au côté serveur](/servo/)

https://www.arducam.com/docs/cameras-for-raspberry-pi/ptz-camera/software/
