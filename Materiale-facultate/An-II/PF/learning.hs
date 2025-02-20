-- https://www.youtube.com/watch?v=TklkNLihQ_A

-- functie factoriala

fact :: Integer -> Integer
fact x | x == 0 = 1 
       | x > 0 = x * fact (x-1)

-- liste
someList = [1,2,3]

-- suma numerelor dintr-o lista
sumList :: Num a => [a] -> a 
sumList [] = 0
sumList (x:xs) = x + sumList xs

takeCustom :: Int -> [a] -> [a]
takeCustom 0 _ = []
takeCustom _ [] = []
takeCustom n (x:xs) = x : takeCustom (n-1) xs


-- pattern matching
-- guards
-- where (clauza) 
-- recursion ( example: factorial )

factorial :: Integer -> Integer 
factorial 0 = 1
factorial n = n * factorial (n-1)

{-

 HIGHER ORDER FUNCTIONS

* map   ( map (+1) [1,2,3] )
* filter  ( filter even [1,2,3,4] )
* foldl    ( foldl (+) 0 [1,2,3,4] )
* foldr     ( foldr (+) 0 [1,2,3,4] )
* sort ( import Data.List (sort))
* zipWith ( zipWith (+) [1,2,3] [4,5,6] )
* takeWhile ( takeWhile (<5) [1..10] )
* dropWhile 
* all/any

-}

-- >>>
-- 1 + 2 + 3 + 4
-- foldl (+) 0 [1,2,3,4]

-- <<<
-- 4 + 3 + 2 + 1
-- foldr (+) 0 [1,2,3,4]

{-
LAMBDA FUNCTION
\ - lambda funtion symbol

( \c -> c * 9/5 + 32) <argument_number>)
-}

{- 
LISTS

* head
* tails
* last
* init
* null
* reverse
* length
* take
* drop
* max/min
* sum
* product
* elem

-}

-- FUNCTION COMPOSITION

fooeven :: Int -> Bool
foonoteven :: Bool -> String

fooeven x = if x `rem` 2 == 0
    then True 
else False

foonoteven x = if x == True 
    then "This is an even number"
else "This is an odd number"

-- main = do 
--     putStrLn "Function composition: "
--     print ((foonoteven.fooeven)(4))


{- 
S II. (2pct) Folosind recursivitate, definiti o functie care are ca argument o lista de cuvinte si calculeaza
numarul total de vocale ce apar in cuvintele palindrom.
-}

-- recursivitate = folosirea unei functii in alta functie (exemplu: factorial)
nrVocale :: [String] -> Int
nrVocale [] = 0
nrVocale (x:xs) | isPalindrom x == True = nrVocala x + nrVocale xs -- prin recursivitate se aduna din nou lista
                | otherwise = nrVocale xs  -- prin recursivitate se intoarce mereu lista 

nrVocala :: String -> Int
nrVocala [] = 0
nrVocala (x:xs) | x `elem` ['A','E','I','O','U','a','e','i','o','u'] = 1 + nrVocala xs
                | otherwise = nrVocala xs

isPalindrom :: String -> Bool
isPalindrom [] = True
isPalindrom (xs)   | xs == reverse xs = True 
                   | otherwise = False

{-
S III. (2pct) Folosind metoda prin selectie (list comprehension), sa se scrie o functie data fiind o lista de
numere calculeaza lista pozitiilor elementelor impare din lista originala.
-}

-- list comprehension = [x | x <- [1..10], x `mod` 2 == 0]

-- [ <output-syntax> | x <- <input-list>, <condition> ]

{- 

output-syntax == the calculation that will be done to the list elements 
    Example: 2*x
input-list == the list that will be used to calculate the output-syntax
    Example: [1..10]
condition == the condition that must be met for the element to be included in the output list
    Example: x `mod` 2 == 0

-}

listPozImpare :: [Int] -> [Int]
listPozImpare [] = []
listPozImpare xs = [ i | (i,x) <- zip [0..] xs, x `mod` 2 /= 0 ]

-- Problema - lista cu 2 argumente, primul argument un caracter iar al 2 lea argument o lista de tupluri formate din un caracter si o lista de numere, se doreste sa se returneze suma numerelor din lista de numere care au ca prim caracter argumentul dat

listaTupluri :: Char -> [(Char, [Int])] -> Int
listaTupluri _ [] = 0
listaTupluri c ((x,y):xs) | c == x = sum y + listaTupluri c xs 
                          | otherwise = listaTupluri c xs

-- Problema - dintr-o lista de liste de numere sa se afiseze o lista cu liste de numere impare 
nestedOdd s = [[ sii | sii <- si, odd sii ] | si <- s ]

-- Problema - dintr-o lista de stringuri, sa se afiseze lista fara whitespace-uri 
removeSpace s = [[  y|y <- x,y /= ' '] | x <- s ]
