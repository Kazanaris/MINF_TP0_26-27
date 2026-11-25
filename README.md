# MINF_TP0_26-27
Ceci est le repo git pour le TP0 de MINF pour l'année 2026-2027.


# Configuration du Timer 1
Dans le Harmony Configurator, sous Harmony Framework Configuration, sous Drivers, sous Timers, le timer est configurer de la façon suivante :
- La case "Use Timer Driver?" est cochée.
- "Driver Implementation" est mit en "STATIC".
- La case "Interrupt Mode" est cochée.
- "Timer Module ID" est mit en "TMR_ID_1".
- "Interrupt Priority" est mit en "INT_PRIORITY_LEVEL3".
- "Prescale" est mit en "TMR_PRESCALE_VALUE_256".
- La valeur 31'249 est inscrite dans "Timer Period".
Les deux prochains points explique le calcule de la valeur du prescaler et le calcule de la valeur du "Timer Period"

## Calcule du prescaler
Le datasheet du PIC32MX795F512L nous indique, au point 13.0 à la page 197 que le timer 1 est un timer 16bit.
Le nombre de "step" maximum qu'il peut compter est de 65'536 (2^16).
La consigne impose que le timer 1 doit faire son interruption toutes les 100ms et notre corps timer est de 80MHz.
Le prescaler se calcule de la façon suivante :
   1) Nbr de step sans prescaler = tVoulu (en microseconde) * fCoreTimer = (100'000 * 10^-6) * (80 * 10^6) = 100'000 * 80 = 8 * 10^6
   2) Prescaler = nbr de step sans prescaler / nbr de step maximum = 8 * 10^6 / 65536 = 122.07
Le prescaler requis est de 122.07 mais il n'existe pas et le timer 1 ne peut utiliser que les prescaler 1, 8, 64 et 256 (dans le datasheet du PIC32 au point 13-1 à la page 199).
Le prescaler sélectionné est donc le prescaler ayant une valeur supérieur à celui calculer, soit 256.

## Calcule du Timer Period
Le "Timer Period" se calcule de la façon suivante :
  1) fApresPerscale = fCoreTimer / prescaler = 80 * 10^6 / 256 = 312.5kHz.
  2) tApresPerscale = 1 / fApresPerscale = 1 / 312.5 * 10^3 = 3.2us.
  3) Timer Period = (tVoulu / tApresPerscale) - 1 = (100 * 10^-3 / 3.2 * 10^-6) - 1 = 31'249.
Dans le dernier calcul, le "- 1" vient du fait que le timer compte de 0 à 65'535, soit 65'536 valeur au total (sur la plage de comptage maximale).

La configuration doit ressembler à cela :
![Texte alternatif](
