-- quicksort
{-# LANGUAGE BlockArguments #-}
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


x :: Integer
x = let z = 5; g u = z + u in let z = 7 in g 0 + z
y :: Integer
y = let z = 5; g u = z + u in let z = 7 in g 0

g x = x

-- f x = g x + g x + z 
--     where 
--         y = 2 * x 
--         z = x - 1 

f x = case x of
     0 -> -2
     1 -> y + 1
     2 -> y * y
     _ -> y
   where
     y = x * x

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
  
