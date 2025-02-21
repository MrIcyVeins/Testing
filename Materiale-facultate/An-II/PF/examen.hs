-- Varianta 1

{-
1.b      f :: (Char,Int) -> Char -> Bool
2.c      [2,1,0,2,1,0,2,1,0,2]
3.a      666
4.a      map (\x -> length x) ["examen","la","PF"]
-}

-- S2 - Folosind recursivitatea ( fct in fct ) definiti o functie care are ca argumente o lista de cuvinte si un caracter si returneaza suma lungimilor cuvintelor care incep cu acel caracter dat ca argument.

incepe :: String -> Char -> Bool 
incepe [] _ = False 
incepe (x:xs) c = if x == c then True else False

recursivitate :: [String] -> Char -> Int
recursivitate [] _ = 0
recursivitate (x:xs) c   | incepe x c =  length x + recursivitate xs c 
                         | otherwise = recursivitate xs c 

-- S3 - Folosind metoda prin selectia (list comprehension), definiti o functie care are ca argument un sir de caractere si o lista de perechi ( cu primul argument un caracter si cel de-al doilea argument un numar ) si calculeaza suma tuturor valorilor ( al 2-lea argument ) perechilor care au caracterul continut in sirul dat ca argument functiei, iar al 2-lea argument este un numar impar.

-- list comprehension  [ x | x <- [], ]

functie :: String -> [(Char, Int)] -> Int
functie _ [] = 0
functie s ((c,x):xs) = sum [ i | (c,i) <- ((c,x):xs), c `elem` s, i `mod` 2 /= 0 ]

--------------------------------------------------------------

-- Varianta 2
{-
1. c
2. d
3. c
4. b
-}

-- S2

recursivitate2 :: Char -> [(Char, Int)] -> Int
recursivitate2 _ [] = 0
recursivitate2 c ((b,x):xs) | c == b = x + recursivitate2 c xs    
                            | otherwise = recursivitate2 c xs 

-- S3
comprehension :: String -> [String] -> Int
comprehension _ [] = 0
comprehension s xs = length [ x | x <- xs, even ( length x ), head x `elem` s  ]

--------------------------------------------------------------
-- Varianta model
-- S2 

estePalindrom :: String -> Bool 
estePalindrom [] = True
estePalindrom x | x == reverse x = True 
                | otherwise = False


recursivitateModel :: [String] -> Int
recursivitateModel [] = 0 
recursivitateModel (x:xs) | estePalindrom x = length x + recursivitateModel xs 
                          | otherwise = recursivitateModel xs 


-- S3 
listaNumere :: [Int] -> [Int]
listaNumere [] = []
listaNumere xs = [ t | (t,i) <- zip [1..] xs, i `mod` 2 /= 0 ]

--------------------------------------------------------------
-- ALTE PROBLEME

-- RECURSION
-- Definiți o funcție recursivă care primește o listă de șiruri de caractere și returnează suma lungimilor șirurilor care conțin cel puțin o vocală (a, e, i, o, u).


vocala :: String -> Bool 
vocala [] = False 
vocala (x:xs)  | x `elem` ['a','e','i','o','u'] = True
               | otherwise = vocala xs

functie1 :: [String] -> Int 
functie1 [] = 0  
functie1 (x:xs) | vocala x = length x + functie1 xs
            | otherwise = functie1 xs 

-- Definiți o funcție recursivă care primește o listă de numere întregi și un număr n și returnează suma numerelor care sunt multipli ai lui n.

functie2 :: [Int] -> Int -> Int 
functie2 [] _ = 0
functie2 (x:xs) n | x `mod` n == 0  = x + functie2 xs n
           | otherwise = functie2 xs n  

-- Definiți o funcție recursivă care primește o listă de perechi (String, Int) și returnează suma valorilor (al doilea element din pereche) pentru care primul element este un palindrom.
-- (palindrom, 3) => 3
-- (nonpalindrom, 5) => 0 
-- (palindrom, 4) => 4 
-- ==> 3 + 0 + 4 

palindrom :: String -> Bool
palindrom [] = True
palindrom x | x == reverse x = True
            | otherwise = False

functie3 :: [(String, Int)] -> Int 
functie3 [] = 0
functie3 ((s,x):xs) | palindrom s = x + functie3 xs  
                 | otherwise = functie3 xs 

-- LIST COMPREHENSION
-- Definiți o funcție care primește un șir de caractere și o listă de perechi (Char, Int) și returnează produsul tuturor valorilor impare unde caracterul apare în șir.

functie4 :: String -> [(Char,Int)] -> Int
functie4 _ [] = 0
functie4 s ((c, x):xs) | not(null s) = sum [ i | (c,i)  <- ((c, x):xs) , odd i, c `elem` s]
                    | otherwise = functie4 s xs

-- Definiți o funcție care primește o listă de șiruri de caractere și returnează numărul de șiruri care au lungime pară și încep cu o consoană (b,c,d,f,g,h,...).

functie5 :: [String] -> Int
functie5 (x:xs) | not (null (x:xs)) = length [ x | x  <- (x:xs), even (length x), (head x) `elem` ['c'..'h'] ]
             | otherwise = functie5 xs


-- Definiți o funcție care primește o listă de numere și returnează lista pozițiilor (1-based) unde se află numere pare.

functie6 :: [Int] -> [Int]
functie6 [] = []
functie6 xs = [ x | (x,i) <- zip [1..] xs, i `mod` 2 == 0 ] 

------ ADV

-- RECURSION

-- Definiți o funcție recursivă care primește o listă de șiruri de caractere și returnează lungimea maximă a unui cuvânt care conține doar consoane.

consoane :: String -> Bool
consoane (x:xs) | x `elem` ['b'..'z'] = True
                | otherwise = False

-- functie7 :: [String] -> Int
-- functie7 (x:xs) | not(null(x:xs)) = maximum [ length x | x <- (x:xs), consoane x ]
--                 | otherwise = functie7 xs 


functie7 :: [String] -> Int 
functie7 [] = 0
functie7 (x:xs) | consoane x = max (length x) (functie7 xs)
                | otherwise = functie7 xs


                
-- Definiți o funcție recursivă care primește o listă de numere întregi și returnează numărul de elemente distincte.

distinct :: Int -> [Int] -> Bool
distinct _ [] = False
distinct x xs | x `elem` xs = True
              | otherwise = False

functie8 :: [Int] -> Int
functie8 [] = 0
functie8 (x:xs) | distinct x xs = functie8 xs
                | otherwise = 1 + functie8 xs

-- Definiți o funcție recursivă care primește o listă și returnează câte ori apare cel mai frecvent element.

frecvent :: Eq a => a -> [a] -> Int
frecvent _ [] = 0 
frecvent x (y:ys) | x == y = 1 + frecvent x ys 
                  | otherwise = frecvent x ys

functie9 :: Eq a => [a] -> Int
functie9 [] = 0
functie9 (x:xs) | not(null(x:xs)) = max (frecvent x (x:xs)) (functie9 xs)
                | otherwise = functie9 xs 

-- LIST COMPREHENSION 

-- Definiți o funcție care primește o listă de nume complete și returnează lista inițialelor numelor care au mai mult de două cuvinte.

-- ["Ion Popescu", "Maria Ionescu", "Ana Pop"] => ["I", "M"]

-- functie10 :: [String] -> [Char] 
-- functie10 [] = []
-- functie10 (x:xs) | not(null(x:xs)) = [ x | x <- (x:xs), ' ' `elem` x]
--                  | otherwise = functie10 xs

-- Definiți o funcție care primește o listă de perechi (String, Int) și returnează un dicționar (Map) unde fiecare cuvânt este asociat cu valoarea maximă dintre cele asociate inițial.

-- Definiți o funcție care primește o listă de numere și returnează lista tuturor submulțimilor cu număr par de elemente.

-- Definiți o funcție care primește o listă de numere și returnează suma cifrelor doar pentru numerele impare.