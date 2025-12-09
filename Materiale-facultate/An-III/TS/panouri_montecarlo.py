import math
import random
import statistics

# 1. Functie: intensitatea solara ideala in functie de ora
#    (fara nori), scalata intre 0 si 1
#    Consideram ziua intre 6:00 si 18:00
# -------------------------------------------------------

def intensitate_ideala(ora):
# Calculeaza intensitatea solara ideala (fara nori) intre 6:00 si 18:00. Rezultatul este o valoare intre 0 si 1

    if ora < 6 or ora > 18:
        return 0.0

    # Normalizam ora in intervalul [0, 1]
    t = (ora - 6) / 12.0

    # Folosim o functie sinus pentru forma tip "clopot":
    intensitate = math.sin(math.pi * t)

    return max(0.0, intensitate)


# 2. Functie: factorul de nori (0 = foarte innorat, 1 = senin)
# -------------------------------------------------------

def factor_nori():
# Genereaza un factor aleator intre 0 si 1 care reprezinta cat de acoperit este cerul

    r = random.random()

    if r < 0.5:
        # ~50% sanse sa fie destul de senin
        return random.uniform(0.7, 1.0)
    elif r < 0.85:
        # ~35% sanse cer partial innorat
        return random.uniform(0.3, 0.7)
    else:
        # ~15% cer foarte innorat
        return random.uniform(0.05, 0.3)


# 3. Functie: simularea energiei produse intr-o singura zi
# -------------------------------------------------------

def simuleaza_o_zi(putere_maxima_kw=1.0, pas_minute=15):
# Simuleaza energia produsa (in kWh) intre 6:00 si 18:00, cu un panou solar de putere maxima specificata

    pas_ore = pas_minute / 60.0
    energie_totala = 0.0

    ora = 6.0
    while ora < 18:
        # intensitatea soarelui fara nori
        intensitate = intensitate_ideala(ora)

        # cat de multi nori sunt
        nori = factor_nori()

        # puterea instantanee produsa de panou
        putere_kw = putere_maxima_kw * intensitate * nori

        # energia produsa in intervalul curent
        energie_totala += putere_kw * pas_ore

        ora += pas_ore

    return energie_totala


# 4. Functie: multe zile simulate (Monte Carlo)
# -------------------------------------------------------

def simuleaza_mai_multe_zile(numar_zile=1000, putere_kw=1.0, pas_minute=15):
# Simuleaza numar_zile zile consecutive si returneaza o lista cu energiile zilnice

    energii = []
    for _ in range(numar_zile):
        e = simuleaza_o_zi(putere_kw, pas_minute)
        energii.append(e)
    return energii


# 5. Program principal
# -------------------------------------------------------

def main():
    print("Simulare Monte Carlo a productiei unor panouri solare\n")

    putere_kw = 10.0       # puterea panoului (kW)
    numar_zile = 1000      # numarul de zile simulate
    pas_minute = 15        # pas de simulare

    print(f"Puterea panoului: {putere_kw} kW")
    print(f"Numar de zile simulate: {numar_zile}")
    print(f"Pas de timp: {pas_minute} minute\n")

    # rulam simularile
    energii = simuleaza_mai_multe_zile(numar_zile, putere_kw, pas_minute)

    # statistici
    medie = statistics.mean(energii)
    minim = min(energii)
    maxim = max(energii)
    abatere = statistics.pstdev(energii)

    print("Rezultate pentru energia produsa pe zi (kWh):")
    print(f"  Medie:        {medie:.2f} kWh/zi")
    print(f"  Minim:        {minim:.2f} kWh/zi")
    print(f"  Maxim:        {maxim:.2f} kWh/zi")
    print(f"  Abatere std.: {abatere:.2f} kWh\n")

if __name__ == "__main__":
    main()
