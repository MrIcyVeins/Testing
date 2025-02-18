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