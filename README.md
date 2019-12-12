# Initiation à la programmation concurrente.

Veillez à avoir une version récente de gcc (7 ou plus)!

Pour compiler sous Linux: `gcc -std=c11 -pthread -Werror -Wall -Wextra ex01.c`
Dans code blocs ajoutez dans settings les options de compilation `-Wall` et
`-Wextra`

Un projet nouveau projet C sans `-Wall` et `-Wextra` en option de compilation,
masque des erreurs graves telle que des variables non initialisées.

## Compte rendu

Rendu: 09 janvier 2019

### Questions de compte rendu

- Combiens de *threads* sont executé au plus par le programme?
- Est-ce que chaque thread possède sa propre pile?
- Est-ce que chaque thread possède son propre tas?
- Combien avons nous de processus?
- Combien il y a t-il *d'espaces d'addressages*?
- Schematiser un ordre d'execution des threads possible.
- Expliquer pourquoi on obtiens pas 30000 à chaque fois
- Realisez un schema pour expliquer un *mauvais scénario* d'accès au compteur
  qui provoque un bug (ne pas avoir 30000 a la fin est un bug)
- Si j'ai 4 processeurs qui peuvent executer un thread, dans le meilleur
des cas combien ai-je de thread executé en parrallèlle.
- Faut t'il des instructions particulières dans le jeu d'instruction (asm)
  pour écrire un lock/mutex?

### Questions de Code

- Corriger le code pour qu'il compile
- Utiliser des Mutex pour protéger le compteur
- Bonus dur: Écrire une structure et son code pour faire un système
  de passage par message (boite au lettres). Un thread lecteur devra écrire les
  messages écrits par 3 threads écrivains
- Bonus très dur: Utiliser des *Atomics* pour faire un compteur atomique partagé
  (compteur de référence atomique)
- Bonus très très dur: Faire un programme qui lis des images au format portable pixmap format
[PGM](https://en.wikipedia.org/wiki/Netpbm_format) et faire en parralelle un
compteur a pixel noirs, le noir est la valeur maximale possible selon l'encodage
de pixel. ex sur 4 bits 255 encode le noir.

