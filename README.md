# POGLA TP1, 2 et 3

Ce programme reuni les features des trois TP de pogla.  
utilisation:
- compilation  
$ make
- execution  
$ ./main
- dans le programme:
    - 'b' comme 'bunny' pour l'animation du lapin
    - 'w' comme 'wave' pour l'animation de la vague
    - 'n' comme 'normals' pour afficher ou non les normales
    - 'a' comme 'angora' pour afficher ou non de la fourrure (plus d'interet sur le lapin mais possible sur la vague)
    - 'p' comme 'points' pour passer de l'affichage de la surface en triangle vers les points et vice versa
    - 't' comme 'timestop' pour arreter ou relancer le cours du temps
    - clique gauche et glisser pour tourner la scene  
petit bug non resolu: Quand on tourne le lapin et que le temps n'est pas arrete la valeur de gonflement va bugger,
le lapin va soit trop gonfler soit se ratatiner. Pour lui faire retrouver sa forme, vous pouver reinitialiser son
animation en rappuyant sur 'b'.
