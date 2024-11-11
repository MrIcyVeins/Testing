-- quicksort
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

maxim4 x y z w =
    let
        u = maxim x y  
    in
        maxim u ( maxim  z w )
