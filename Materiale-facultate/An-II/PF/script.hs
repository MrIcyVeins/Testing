-- quicksort
{-# LANGUAGE BlockArguments #-}
import Data.Binary.Get (label)
qsort :: Ord a => [a] -> [a]

qsort [] = []
qsort (p:xs) = filter (< p) xs  ++ [p] ++ filter (>= p) xs

trei = let {a=1; b=2} in a + b

patru = let a=1; b=3 in a + b


ceva :: String
ceva = let a="ce"; b="va" in a ++ b

altceva :: String
altceva = let
  a = "alt"
  b = "ceva"
 in a ++ b


-- x :: Integer
-- x = let z = 5; g u = z + u in let z = 7 in g 0 + z
-- y :: Integer
-- y = let z = 5; g u = z + u in let z = 7 in g 0

-- g x = x

-- f x = g x + g x + z 
--     where 
--         y = 2 * x 
--         z = x - 1 

-- f x = case x of
--      0 -> -2
--      1 -> y + 1
--      2 -> y * y
--      _ -> y
--    where
--      y = x * x

h x
  | x == 0  = 0
  | x == 1  = y + 1
  | x == 2  = y * y
  | otherwise = y
      where y = x*x

maxim :: Integer -> Integer -> Integer
maxim = max

test x y z
  | x > y && x > z = x
  | y > x && y > z = y
  | otherwise = z

maxim3 x y z =
    let
       u = maxim x y
    in
       maxim u z

-- Scrieti o functie, maxim4 folosind varianta cu let..in, si indentare.

maxim4 x y z w =
    let
        u = maxim x y  
    in
        maxim u ( maxim  z w )


fact n 
  | n == 0 = 34
  | otherwise = n * fact (n-1)


-- Conditii = garzi
semn :: Integer -> Integer
semn n 
  | n < 0 = -1
  | n == 0 = 0 
  | otherwise = 1

-- Versiunea cu sabloane

semn1 :: Integer -> Integer
semn1 0 = 0 
semn1 x
  | x > 0 = 1
  | otherwise = -1

-- triple x y z = let
--              u = maxim x y 
--              in (maxim u z)

-- m :: (Char -> Bool) -> Char -> Bool
-- m f c = f c

-- f x = let x = 3 ; y = 4 in x + y


{- Exercitii

a) functie cu 2 parametri care calculeaza suma patratelor celor doua numere
b) functie cu un parametru ce intoarce mesajul "par" daca parametrul este par si "impar" altfel
c) functie care verifica daca primul parametru este mai mare decat dublul celui de-al doilea parametru

-}

-- a)
-- functie :: (Integer -> Integer) -> Integer
functieA x y = x^2 + y^2

-- b)
functieB x 
  | x < 0 = "impar"
  | x > 0 = "par"

-- c)
functieC x y 
  | x > y*y = True
  | otherwise = False


{- Exercitii
a) Scrieti o functie cu 2 parametri care calculeaza suma patratelor celor doua numere folosind sectiuni
b) Scrieti o functie cu 3 parametri care verifica daca se poate forma un triunghi dreptunghic avand lungimile laturilor de cei 3 parametri. Rezolvati acest exercitiu folosind sectiuni.
c) Scrieti o functie poly care are patru argumente de tip Double, a,b,c,x si calculeaza a * x^2 + b * x + c. Scrieti si signatura functiei (poly :: ceva)
d) Scrieti o functie eeny care intoarce "eeny" pentru input par si "meeny" pentru input impar. Hint: se poate folosi functia even 
e) Scrieti o functie fizzbuzz care intoarce "Fizz" pentru numerele divizibile cu 3, "Buzz" pnetru numerele divizibile cu 5 si "FizzBuzz" pentru numerele divizibile cu ambele. Pentru orice alt numar se intoarce sirul vid. Pentru a calcula moduloa a doua numere puteti folosi functia mod. Sa se scrie aceasta functie in 2 moduri: folosind if si folosind garzi ( conditii )
-}

-- a)
functieO x y = let in (^2) x + (^2) y 
-- sau 
a = (^2) ; b x y = a x + a y 

-- b)

-- I = C1 + C2
s = (^2)
functieS x y z 
  | (^2) x == (^2) y + (^2) z  = "true"
  | (^2) y == (^2) x + (^2) z  = "true"
  | (^2) z == (^2) x + (^2) y  = "true"
  | otherwise = "false"

-- c)
poly :: Double -> Double -> Double -> Double -> Double
poly a b c x = let in a * x^2 + b * x + c

-- d)
eeny x 
  | even x  = "eeny"
  | not(even x)  = "meeny" -- sau odd x

-- e)
-- fizzbuzz :: (Integer -> Integer) -> String
fizzbuzz x 
  | mod x 3 == 0 && mod x 5 == 0 = "FizzBuzz"
  | mod x 3 == 0 = "Fizz"
  | mod x 5 == 0 = "Buzz"
  | otherwise = ""
  
fibonaci :: Integer -> Integer 
fibonaci n 
  | n < 2 = n
  | otherwise = fibonaci (n-1) + fibonaci (n-2)


-- In stil ecuational ( cu sabloane )
fibonacciEcuational :: Integer -> Integer 
fibonacciEcuational 0 = 0
fibonacciEcuational 1 = 1 
fibonacciEcuational n = fibonacciEcuational (n - 1) + fibonacciEcuational (n - 2)

{- 
Exercitiul 4.1 

- Completati definitia functiei fibonacciPereche

Folositi principiul de inductie: ne bazam pe faptul ca fibonacciPereche (n-1) va calcula perechea (F n-2, F n-1) si o folosim pe aceasta pentru a calcula perechea (Fn-1, Fn).

Observatie 2. Recursia este liniara doar daca expresia care reprezinta apelul recursiv apare o singura data. Folositi let, case sau where pentru a va asigura acest lucru.
-}


fibonacciLiniar :: Integer -> Integer 
fibonacciLiniar 0 = 0 
fibonacciLiniar n = snd (fibonacciPereche n)
  where 
    fibonacciPereche :: Integer -> (Integer, Integer)
    fibonacciPereche 1 = (0,1)
    fibonacciPereche n = undefined

{- 
Exercitiul 4.2

Sa se implementeze functia tribonacci atat cu cazuri cat si ecuational

tribonacci :: Integer -> Integer
tribonacci = undefined

fibonaci n 
  | n < 2 = n
  | otherwise = fibonaci (n-1) + fibonaci (n-2)

fibonacciEcuational :: Integer -> Integer 
fibonacciEcuational 0 = 0
fibonacciEcuational 1 = 1 
fibonacciEcuational n = fibonacciEcuational (n - 1) + fibonacciEcuational (n - 2)
-}

tribonacci :: Integer -> Integer 
-- tribonacci = undefined 

-- sabloane
tribonacci n
  | n <= 2 = 1
  | n <= 3 = 2
  | otherwise = tribonacci (n-1) + tribonacci (n-2) + tribonacci (n-3)

-- ecuational 
tribonacciEcuational :: Integer -> Integer 
tribonacciEcuational 1 = 1
tribonacciEcuational 2 = 1
tribonacciEcuational 3 = 2
tribonacciEcuational n = tribonacciEcuational (n-1) + tribonacciEcuational (n-2) + tribonacciEcuational (n-3)

{- 
Exercitiul 4.3
Sa se scrie o functie care calculeaza coeficientii binomiali, folosind recursivitate. Acestia sunt determinati folosind urmatoarele ecuatii.

B(n,k) = B(n-1,k) + B(n-1,k-1)
B(n,0) = 1
B(0,k) = 0

binomial :: Integer -> Integer -> Integer
binomial = undefined
-}

binomial :: Integer -> Integer -> Integer
binomial _ 0 = 1   -- B(n,0)
binomial 0 _ = 0   -- B(0,k)
binomial n k
  | k == n    = 1  -- cazul special: B(n, n) = 1
  | otherwise = binomial (n - 1) k + binomial (n - 1) (k - 1)

-- Functii anonime ( Lambda Expresii )
inc = \x -> x + 1 
add = \x y -> x + y
aplic = \f x -> f x

-- Compunerea functiilor - operatorul
{- 

MATEMATIC
f: A -> B
g: B -> C
g o f  : A -> C 

(g o f )(x) = g(f(x))

HASKELL
(.) :: (b -> c) -> (a -> b) -> (a -> c)
(g . f) x = g (f x)

-}  

g :: Int -> Bool
g x = x > 0 
f :: Int -> Int 
f x = x + 1

{-
Exercitiul 6.1

Definiti 2 functii care sa calculeze dublul si triplul unui numar si gasiti toate modalitatile de compunere a acestor functii.
q,w :: Int -> Int
-}

q,w :: Int -> Int
q x = x * 2
w x = x * 3

{- 
Compuneri:

(w . w) x
(w . q) x
(q . w) x
(q . q) x
-}

{-
Liste 

- se pot scrie doar prin constructorul : si lista vida []
- o lista este > vida []
               > compusa x:xs  ( x - capul listei / xs - coada listei )
-} 


-- Exercitiu - data fiind o lista de numere intregi, sa se scrie o functie semiPareRec care elimina numerele impare si le injumatateste pe cele pare.

-- semiPareRec [0,2,1,7,8,56,17,18] == [0,1,4,28,9]

semiPareRec :: [Int] -> [Int]
semiPareRec [] = []
semiPareRec (h:t)
  | even h = h `div` 2 :t'
  | otherwise = t'
      where t' = semiPareRec t 

-- reverse [0,1,2] == [2,1,0]

rev :: [Int] -> [Int]
rev [] = []
rev (h:t) = rev (rev t) ++ [h]

{- 
Exercitiul 7.1 
a) myreplicate - pentru un intreg n si o valoare v intoarce lista de lungime n ce are doar elemente egale cu v. Sa se scrie si prototipul functiei. ( functie :: data -> data -> ...)
b) sumImp - pentru lista numere intregi, sa se calculeze suma valorilor impare.
c) totalLen - pentru o lista de siruri de caractere, calculeaza suma lungimilor sirurilor care incep cu caracterul 'A'
d) nrVocale - pentru o lista de siruri de caractere, calculeaza numarul total de vocale ce apar in cuvintele palindrom. Pentru a verifica daca un sir e palindrom, puteti folosi functia 'reverse', iar pentru a cauta un element intr-o lista puteti folosi functia 'elem'. Puteti defini oricate functii auxiliare.
e) 

-}
-- a) + prototipul functiei
myreplicate :: Int -> Int-> [Int]
myreplicate n v 
  | n == 0 = []
  | n > 0 = concatMap (replicate n) [v]

-- b) + prototipul functiei ( par = div 2 / impar not div 2)
sumImp :: [Int] -> Int
sumImp [] = 0
sumImp (x:xs)
  | odd x      = x + sumImp xs
  | otherwise  = sumImp xs

-- c) totalLen
totalLen :: [String] -> Int
totalLen [] = 0
totalLen (x:xs)
  | not (null x) && head x == 'A' = t' + totalLen xs
  | otherwise  = totalLen xs
    where t' = length x