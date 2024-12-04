import Data.List.NonEmpty (xor)
-- https://wiki.haskell.org/H-99:_Ninety-Nine_Haskell_Problems


-- Questions 1 to 10: Lists
-- Problem 1 (*)
-- Find the last element of a list
{-  

λ> myLast [1,2,3,4]
4
λ> myLast ['x','y','z']
'z'

-}

myLast :: [a] -> a
myLast [] = error "exception"
myLast (x:xs) = last xs

-- Problem 2 
{-

Find the last-but-one (or second-last) element of a list

λ> myButLast [1,2,3,4]
3
λ> myButLast ['a'..'z']
'y'

-}

myButLast :: [a] -> a
myButLast (x:xs)
    | not(null xs) = last t' 
        where t' = init xs

-- Problem 3
{-  

Find the K'th element of a list

λ> elementAt [1,2,3] 2
2
λ> elementAt "haskell" 5
'e'

-}

elementAt :: [a] -> Int -> a
elementAt (x:xs) n
    | n <= 0 = error "0'th element doesn't exist"
    | n == 1 = x 
    | not(null xs) = t'
        where t' = ( x : xs ) !! (n-1)

-- Problem 4
{-  

Find the number of elements in a list

λ> myLength [123, 456, 789]
3
λ> myLength "Hello, world!"
13

-}

myLength :: [a] -> Int
myLength [] = 0
myLength (x:xs)
    | not (null (x : xs)) = length ( x : xs )

-- Problem 5
{- 

Reverse a list 

λ> myReverse "A man, a plan, a canal, panama!"
"!amanap ,lanac a ,nalp a ,nam A"
λ> myReverse [1,2,3,4]
[4,3,2,1]

-}

myReverse :: [a] -> [a]
myReverse [] = []
myReverse (x:xs)
    | not (null (x:xs)) = reverse ( x : xs )

-- Problem 6 (*)
{- 

Find out whether a list is a palindrome

Hint: A palindrome can be read forward or backward; e.g. (x a m a x).

λ> isPalindrome [1,2,3]
False
λ> isPalindrome "madamimadam"
True
λ> isPalindrome [1,2,4,8,16,8,4,2,1]
True

-}

isPalindrome :: Eq a => [a] -> Bool
isPalindrome [] = False
isPalindrome (x:xs)
    | (x:xs) == reverse(x:xs) = True
    | otherwise = False

-- Problem 7 - used helpers
{- 

(**) Flatten a nested list structure

Transform a list, possibly holding lists as elements into a `flat' list by replacing each list with its elements (recursively).

Example:

* (my-flatten '(a (b (c d) e)))
(A B C D E)

We have to define a new data type, because lists in Haskell are homogeneous.

 data NestedList a = Elem a | List [NestedList a]

λ> flatten (Elem 5)
[5]
λ> flatten (List [Elem 1, List [Elem 2, List [Elem 3, Elem 4], Elem 5]])
[1,2,3,4,5]
λ> flatten (List [])
[]

-}

-- with concatMap
-- data NestedList a = Elem a | List [NestedList a]
-- flatten :: NestedList a -> [a]
-- flatten (Elem x) = [x]
-- flatten (List x) = concatMap flatten x

-- without concatMap
data NestedList a = Elem a | List [NestedList a]
flatten :: NestedList a -> [a]
flatten (Elem a )   = [a]
flatten (List (x:xs)) = flatten x ++ flatten (List xs)
flatten (List [])     = []

-- Problem 8 - used helpers
{- 

Eliminate consecutive duplicates of list elements
λ> compress "aaaabccaadeeee"
"abcade"

-}
-- data Elements a =  [a]
compress :: Eq a => [a] -> [a]
compress [] = []  -- Handle the empty list case
compress (x:xs) = x : (compress $ dropWhile (== x) xs)

-- Problem 9
{-
Pack consecutive duplicates of list elements into sublists.
λ> pack ['a', 'a', 'a', 'a', 'b', 'c', 'c', 'a', 
             'a', 'd', 'e', 'e', 'e', 'e']
["aaaa","b","cc","aa","d","eeee"]

-}

pack :: Eq a => [a] -> [a]
pack [] = []
pack (x:xs) = (x : (pack $ takeWhile (==x) xs)) ++ (x : (pack $ takeWhile (==x) xs))



-- Problem 10
{- 
Run-length encoding of a list.

λ> encode "aaaabccaadeeee"
[(4,'a'),(1,'b'),(2,'c'),(2,'a'),(1,'d'),(4,'e')]

-}