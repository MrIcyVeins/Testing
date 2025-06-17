% Predicat distance/3 - calculeaza distanta dintre doua puncte din plan, puncte = perechi de coordonate

distance((X1, Y1), (X2, Y2), D) :-
    DX is X2 - X1,
    DY is Y2 - Y1,
    D2 is DX * DX + DY * DY,
    D is sqrt(D2). 

parent(bob, lisa).
parent(bob, paul).
parent(bob, mary).
parent(juliet, lisa).
parent(juliet, paul).
parent(juliet, mary).
parent(peter, harry).
parent(lisa, harry).
parent(mary, dony).
parent(mary, sandra).

% ancestor_of(X,Y) - adevarat daca X este un stramos al lui Y
% Definitia recursiva a predicatului:

% prima regula de baza
ancestor_of(X,Y) :- parent(X,Y). 
% a doua regula recursiva 
ancestor_of(X,Y) :- parent(X,Z), ancestor_of(Z,Y).

% Fibonacci
/*
fib/2 

F0 := 1
F1 := 1
F2 := 2
F3 := 3
F4 := 4
F5 := 5

F4+F3+F2+F1+F0
m >= 2 

Fn = Fn-1 + Fn-2

fib(1,X).
X=1

fib(5,X)
X=8

N=
fib(N,X) :- 
*/

fib(0,1).
fib(1,1).
fib(N,X) :- 2 =< N, M is N - 1, fib(M, Y), P is N - 2, fib(P, Z), X is Y + Z.

fibo(0,0,1).
fibo(1,1,1).
fibo(N,Z,X) :- 2 =< N, M is N-1, fibo(M,Y,Z), X is Y + Z.

fibg(N,X) :- fibo(N,_,X).

/*
element_of/2 - predicat care verifica daca o lista contine un anumit termen

element_of(X,Y) - true daca X este element al lui Y
*/

element_of(X, [X|_]).

element_of(X,[_|Tail]) :- element_of(X,Tail).

/*
Predicat all_a/1 - primeste ca argument o lista care verifica daca argumentul sau este format doar din a-uri
*/

% all_a([_|_]) :- element_of(a,[a|_]), element_of(a,[_|Tail]). 

all_a([]).
all_a([a|Tail]) :- all_a(Tail).

/* sau pentru a */

all_b([]).
all_b([X|Tail]) :- X == b, all_b(Tail).

/*
Predicat trans_a_b/2 care traduce o lista de a-uri intr-o lista de b-uri
trans_a_b(X,Y) trebuie sa fie adevarat daca 
- X este o lista de a-uri
- Y este o lista de b-uri
- cele 2 au lungimi egale 

trans_a_b([a,a,a],L).
trans_a_b([a,a,a],[b]).
trans_a_b(L,[b,b]).

V1

trans_a_b([],[]).
trans_a_b([X|Y],[Z|M]) :- X == a, Z == b, trans_a_b(Y,M),
    length([X|Y],L),
    length([Z|M],C),
    L == C.

V2

trans_a_b([],[]).
trans_a_b([a|Y],[b|M]) :- trans_a_b(Y,M),
    length([a|Y],L),
    length([b|M],C),
    L == C.
*/


/* V3 fara redundanta - face recursivitate => automat liste egale */
trans_a_b([],[]).
trans_a_b([a|Y],[b|M]) :- trans_a_b(Y,M).



/* 

Operatii cu vectori

scalarMult/3
arg 1 = A
arg 2 = []
arg 3 = A * []

Ex: scalarMult (3,[2,7,4],Result).

[6,21,12]

GRESIT
scalarMult(A,[],Result).
scalarMult(A,[X|Tail],[Result]) :- 
    Result is A * X,
    Result is scalarMult(A,[_|Tail],[Result]).

CORECT
scalarMult(_,[],[]).
scalarMult(A, [X|Xs], [Y|Ys]) :-
    Y is A * X,
    scalarMult(A, Xs, Ys).

*/

scalarMult(_,[],[]).
scalarMult(A,[X|Tail],[R|Result]) :- 
    R is A * X,
    scalarMult(A,Tail,Result).


/*
predicat dot/3
arg1 - []
arg2 - [] - length = arg1
arg3 - arg1 * arg2

dot([2,5,6],[3,4,1],Result).
Result = 32

*/

dot([],[],0).
dot([A|B],[C|D],Result) :-
    dot(B,D,R),
    Result is A * C + R.
