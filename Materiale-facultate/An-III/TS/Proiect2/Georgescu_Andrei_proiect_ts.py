import math
import random

# -------------------------
# 1) Gamma standard Gama(0,1,nu) cu infasuratoare exponentiala (nu > 1)
# -------------------------
def gamma_std_exp_envelope(nu: float) -> float:
    """
    Genereaza X ~ Gama(0,1,nu) pentru nu > 1
    folosind infasuratoare exponentiala h(x) = (1/nu) * exp(-x/nu).
    (curs 5, sectiunea pentru nu>1)
    """
    if nu <= 1:
        raise ValueError("Algoritmul cu infasuratoare exponentiala cere nu > 1.")

    # constanta alpha = r(nu) = nu^nu * e^(1-nu) / Gamma(nu)
    alpha_const = (nu ** nu) * math.exp(1 - nu) / math.gamma(nu)

    while True:
        # Y ~ Exp(rate = 1/nu) => Y = -nu * ln(U)
        u1 = random.random()
        y = -nu * math.log(u1)

        # r(y) = f(y)/h(y)
        # f(y) = (1/Gamma(nu)) * y^(nu-1) * exp(-y)
        # h(y) = (1/nu) * exp(-y/nu)
        r = (nu * (y ** (nu - 1)) * math.exp(-y + y / nu)) / math.gamma(nu)

        u2 = random.random()
        if u2 <= (r / alpha_const):
            return y


def gamma(alpha: float, lam: float, nu: float) -> float:
    """
    Genereaza Y ~ Gama(alpha, lam, nu) folosind reducerea:
    Y = alpha + X/lam, unde X ~ Gama(0,1,nu)
    """
    x = gamma_std_exp_envelope(nu)  # X ~ Gama(0,1,nu)
    return alpha + x / lam


# Cerinta: Gama(0,4,6)
def gamma_0_4_6() -> float:
    return gamma(alpha=0.0, lam=4.0, nu=6.0)


# -------------------------
# 2) Hipergeometrica (A albe, B negre, n extrageri fara intoarcere)
# -------------------------
def hypergeometric(A: int, B: int, n: int) -> int:
    """
    Genereaza X = nr bile albe extrase, la n extrageri fara intoarcere.
    Algoritm direct din curs 7.
    """
    if A < 0 or B < 0:
        raise ValueError("A si B trebuie sa fie >= 0.")
    N = A + B
    if n < 0 or n > N:
        raise ValueError("Trebuie 0 <= n <= A+B.")

    x = 0
    p = A / N if N > 0 else 0.0

    for _ in range(n):
        u = random.random()
        if u < p:
            x += 1
            A -= 1
        else:
            B -= 1

        N -= 1
        p = (A / N) if N > 0 else 0.0

    return x


# -------------------------
# 3) Validare: medie si dispersie de selectie
# -------------------------
def sample_mean_var(samples):
    n = len(samples)
    mean = sum(samples) / n
    mean_sq = sum(x * x for x in samples) / n
    var = mean_sq - mean * mean
    return mean, var


def validate_gamma_0_4_6(num_samples=10000):
    samples = [gamma_0_4_6() for _ in range(num_samples)]

    mean_emp, var_emp = sample_mean_var(samples)

    # teoretic pentru Gama(0,4,6):
    mean_th = 6.0 / 4.0
    var_th = 6.0 / (4.0 ** 2)

    print("=== Validare Gama(0,4,6) ===")
    print(f"n = {num_samples}")
    print(f"Media teoretica:      {mean_th}")
    print(f"Media de selectie:    {mean_emp}")
    print(f"Dispersia teoretica:  {var_th}")
    print(f"Dispersia de selectie:{var_emp}")


def validate_hypergeometric(A, B, n, num_samples=10000):
    samples = [hypergeometric(A, B, n) for _ in range(num_samples)]

    mean_emp, var_emp = sample_mean_var(samples)

    N = A + B
    p = A / N
    mean_th = n * p
    var_th = n * p * (1 - p) * ((N - n) / (N - 1)) if N > 1 else 0.0

    print("=== Validare Hipergeometrica ===")
    print(f"Parametri: A={A}, B={B}, N={N}, n={n}")
    print(f"n_selectie = {num_samples}")
    print(f"Media teoretica:      {mean_th}")
    print(f"Media de selectie:    {mean_emp}")
    print(f"Dispersia teoretica:  {var_th}")
    print(f"Dispersia de selectie:{var_emp}")


if __name__ == "__main__":
    # Ruleaza validarile:
    validate_gamma_0_4_6(num_samples=20000)

    # Exemplu pentru hipergeometrica (poti schimba):
    validate_hypergeometric(A=20, B=30, n=10, num_samples=20000)
