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

/* C01 - C02 */
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

/*
predicat max/2 
cauta elem maxim intr-o lista de nr naturale


max([4,2,6,8,1],Result).
Result = 8

*/

max([],0).
max([A|B],Result) :-
    max(B,R),
    (A>R -> Result = A ; Result = R).

/* comparatie -> Rezultat = A ; else Rezultat = R  */

/* C03 */

% Alte exercitii cu liste

/* 
predicat palindrome/1
- adevarat daca lista (arg1) este palindrom

*/

rev([],[]).
rev([A|B],D) :-
    rev(B,C),
    append(C,[A],D).

% append ( adauga elementul A din D in C )         C <- A din D 

palindrome([]).
palindrome(A) :-
    rev(A,R),
    A = R.

/*
remove_duplicates/2
- sterge toate duplicatele din lista data (arg1)
- rezultat in al 2 lea argument (arg2)

remove_duplicates([a,b,c,d,d],List).
List=[b,a,c,d].


remove_duplicates([],[]).
remove_duplicates([A|[Head|B]],C) :-
    remove_duplicates([B],D),
    (A = Head -> append(D,[A],[Head|B]) ; C = D).
*/

remove_duplicates([],[]).
remove_duplicates([A|B],List) :-
    remove_duplicates(B,List),
    member(A,List).

remove_duplicates([A|B],[A|List]) :-
    remove_duplicates(B,List),
    not(member(A,List)).

/*
atimes/3
elem primul arg apare in lista din al 2 lea arg de nr de ori prec in al 3 lea arg


atimes(_,[],0).
atimes(N,[N|T],X) :- atimes(N,T,Y), X is Y + 1.
atimes(N,[H|T],X) :- atimes(N,T,X), H \= N.

*/

atimes(_,[],0).
atimes(A,[A|C],D) :- atimes(A,C,N), D is N + 1.
atimes(A,[E|C],D) :- atimes(A,C,D), E\= A.


/*
Sortari
insertsort/2
- sorteaza lista de pe primul arg folosind algoritmul insertion sort



insertsort([],[]).
insertsort([H|T],L) :- insertsort(T,L1), insert(H,L1,L).

REZ ( insert/3 )
insert(X,[],[X]).
insert(X,[H|T],[X|[H|T]]) :- X < H.
insert(X,[H|T],[H|L]) :- X >= H, insert(X,T,L).
*/
 

/*
quicksort/2
sorteaza lista de pe primul arg cu alg quicksort

quicksort([],[]).
quicksort([H|T],L) :-
split(H,T,A,B), quicksort(A,M), quicksort(B,N),
append(M,[H|N],L).

split/4

split(_,[],[],[]).
split(X,[H|T],[H|A],B) :- H < X, split(X,T,A,B).
split(X,[H|T],A,[H|B]) :- H >= X, split(X,T,A,B).

*/

split(_,[],[],[]).
split(X,[H|T],[H|A],B) :- H < X, split(X,T,A,B).
split(X,[H|T],A,[H|B]) :- H >= X, split(X,T,A,B).

quicksort([],[]).
quicksort([H|T],L) :-
    split(H,T,A,B), quicksort(A,M), quicksort(B,N),
    append(M,[H|N],L).

/*
Arbori binari

De 2 feluri:
- vid
- nevid ( radacina + subarbori )

srd/2
- parcurgerea arb binar in inordine

Teste:

A = arb(5, arb(3, vid, vid), arb(7, vid, vid)).
srd(A,L).
*/

srd(vid,[]).
srd(arb(R,T,U),L) :- 
    srd(T,L1), srd(U,L2), append(L1,[R|L2],L).

/*
Arbori binari de cautare
bt_ins/3 - insereaza nr nat din primul arg in arb binar de cautare din al 2 lea arg
bt_list/2 - transofrma lista din primul arg intr-un arbore binar de cautare
bt_sort/2 - sorteaza lista din primul arg trecand prin arborele binar de  cautare asociat ei

bt_ins(N,vid,arb(N,vid,vid)).
bt_ins(N,arb(R,T,U),arb(R,Z,U)) :- N < R, bt_ins(N,T,Z).
bt_ins(N,arb(R,T,U),arb(R,T,Z)) :- N >= R, bt_ins(N,U,Z).

bt_list([],vid).
bt_list([H|T],U) :- bt_list(T,B), bt_ins(H,B,U).

bt_sort(L,M) :- bt_list(L,T), srd(T,M).

*/

bt_ins(N,vid,arb(N,vid,vid)).
bt_ins(N,arb(R,T,U),arb(R,Z,U)) :- N < R, bt_ins(N,T,Z).
bt_ins(N,arb(R,T,U),arb(R,T,Z)) :- N >= R, bt_ins(N,U,Z).

bt_list([],vid).
bt_list([H|T],U) :- bt_list(T,B), bt_ins(H,B,U).

bt_sort(L,M) :- bt_list(L,T), srd(T,M).

/*
C04 - Unificare

x,y,z,u,v,w - variabile
a,b,c - constante
h,g - simboluri de functie de aritate 1 
f - simbol fct aritate 2
p - simbol fct aritate 3 


V = {x0,x1...} - multime de variabile
V ∩{⊥,→,∀,=}= ∅ - V nu se intersecteaza cu obiectele ( ⊥, →, ∀, = )

ALGORITMUL DE UNIFICARE


intrare = E - multime de ecuatii
iesire = cgu ( dat de o multime de ecuatii ) sau esec daca un cgu nu exista 

algoritmul trece printr-un ciclu care:
> la fiecare pas - verifica daca exista vreo ecuatie in multime caruia i se poate aplica vreuna dintre urmatoarele operatii 
- in caz contrar se opreste

Considerăm
- x, y, z, u, v, w variabile,
- a, b, c constante,
- h, g simboluri de funcţie de aritate 1,
- f simbol de funcţie de aritate 2,
- p simbol de funcţie de aritate 3.
Aplicaţi algoritmul de unificare din curs pentru a găsi un unificator pentru termenii:
1. f (x, y), f (h(x), x)şi f (x, b)
2. f (x, f (x, g(y))), f (u, z)şi f (g(y), y)
3. f (f (x, y), x), f (g(y), z)şi f (u, h(z))
4. f (f (x, y), x), f (v, u)şi f (u, h(z))
5. f (f (x, y), x), f (v, u)şi f (u, z)
6. f (f (g(x), h(y)), h(z)), f (f (u, h(h(x))), h(y))şi f (v, w)
7. p(x, x, z), p(f (a, a), y, y)şi p(f (x, a), b, z)
8. p(x, x, z), p(f (a, a), y, y)şi p(x, b, z)
9. p(x, x, z), p(f (a, a), y, y)şi p(x, f (a, a), z)
10. p(f (x, a), g(y), z), p(f (a, a), z, u)şi p(v, u, z)
1



*/

/*  ex 1 - f(x, y), f(h(x), x), f(x, b) */
rezolvare1 :-
    f(X, Y) = f(h(X), X),
    f(h(X), X) = f(X, b),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl.

/* f(x, f (x, g(y))), f (u, z)şi f (g(y), y) */
rezolvare2 :-
    f(X, f(X, g(Y))) = f(U, Z),
    f(U, Z) = f(g(Y), Y),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('U = '), write(U), nl,
    write('Z = '), write(Z), nl.

/* f (f (x, y), x), f (g(y), z)şi f (u, h(z)) */
rezolvare3 :-
    f(f(X, Y), X) = f(g(Y), Z),
    f(g(Y), Z) = f(U, h(Z)),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl,
    write('U = '), write(U), nl.

/* f(f (x, y), x), f (v, u)şi f (u, h(z))  */

rezolvare4 :-
    f(f(X, Y), X) = f(V, U),
    f(U, h(Z)) = f(U, h(Z)),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('V = '), write(V), nl,
    write('U = '), write(U), nl,
    write('Z = '), write(Z), nl.

/* f (f (x, y), x), f (v, u)şi f (u, z) */
rezolvare5 :-
    f(f(X, Y), X) = f(V, U),
    f(U, Z) = f(U, Z),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('V = '), write(V), nl,
    write('U = '), write(U), nl,
    write('Z = '), write(Z), nl.

/* f (f (g(x), h(y)), h(z)), f (f (u, h(h(x))), h(y))şi f (v, w) */
rezolvare6 :-
    f(f(g(X), h(Y)), h(Z)) = f(f(U, h(h(X))), h(Y)),
    f(f(U, h(h(X))), h(Y)) = f(V, W),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl,
    write('U = '), write(U), nl,
    write('V = '), write(V), nl,
    write('W = '), write(W), nl.

/* p(x, x, z), p(f (a, a), y, y)şi p(f (x, a), b, z) */
rezolvare7 :-
    p(X, X, Z) = p(f(a, a), Y, Y),
    p(f(a, a), Y, Y) = p(f(X, a), B, Z),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl,
    write('B = '), write(B), nl.

/* p(x, x, z), p(f (a, a), y, y)şi p(x, b, z) */
rezolvare8 :-
    p(X, X, Z) = p(f(a, a), Y, Y),
    p(f(a, a), Y, Y) = p(X, b, Z),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl.

/* p(x, x, z), p(f (a, a), y, y)şi p(x, f (a, a), z) */
rezolvare9 :-
    p(X, X, Z) = p(f(a, a), Y, Y),
    p(f(a, a), Y, Y) = p(X, f(a, a), Z),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl.

/* p(f (x, a), g(y), z), p(f (a, a), z, u)şi p(v, u, z) */
rezolvare10 :-
    p(f(X, a), g(Y), Z) = p(f(a, a), Z1, U),
    p(f(a, a), Z1, U) = p(V, U, Z),
    write('X = '), write(X), nl,
    write('Y = '), write(Y), nl,
    write('Z = '), write(Z), nl,
    write('Z1 = '), write(Z1), nl,
    write('U = '), write(U), nl,
    write('V = '), write(V), nl.

/* 
C05 - Rezolutie

Considerăm
- x, y, z, u, v, w variabile,
- a, b, c constante,
- h, g simboluri de funcţie de aritate 1,
- f simbol de funcţie de aritate 2,
- p simbol de funcţie de aritate 3.
Găsiţi o SLD-respingere pentru următoarele programe Prolog şi ţinte:
1.
r :- p,q. t. ?- w.
s :- p,q. q.
v :- t,u. u.
w :- v,s. p.
2.
q(X,Y) :- q(Y,X), q(Y,f(f(Y))). ?- q(f(Z),a).
q(a,f(f(X))).
3.
p(X) :- q(X,f(Y)), r(a). r(X) :- q(X,Y). ?- p(X), q(Y,Z).
p(X) :- r(X). r(f(b)).
q(X,Y) :- p(Y).


*/

r :- p,q. t.
s :- p,q. q.
v :- t,u. u.
w :- v,s. p.


/*
C06 - formule propozitionale - formul este/sau nu tautologica - PROBLEME LEGATE

Exercitii:
vars/2
- adevarat - primul arg = formula + al 2 lea arg = [var1,var2..varn]

Ex:
vars(imp(non(a),imp(a,b)),S)
S = [a,b]

> folositi atom/1
> folositi union/3 - calc reuniunea a 2 liste.

atom(X) - verifică dacă X este un atom (ex: a, b, x) ⇒ e tratat ca variabilă logică (nu în sensul Prolog, ci simbol logic)
non(X) - negația
si(X,Y) - conjuncția (∧)
sau(X,Y) - disjuncția (V)
imp(X,Y) - implicația (→)
*/

vars(X,[X]) :- atom(X).
vars(non(X),S) :- vars(X,S).
vars(si(X,Y),S) :- vars(X,T), vars(Y,U), union(T,U,S).
vars(sau(X,Y),S) :- vars(X,T), vars(Y,U), union(T,U,S).
vars(imp(X,Y),S) :- vars(X,T), vars(Y,U), union(T,U,S).


/* 

predicat all_evals/2

- foromula X  - pentru orice As, all_evals(X,As) = adevarat atunci cand As = [] rez ev lui X in fiecare dintre elem listei eval def pe Var(X)
Exemple:
?- all evals(imp(a,a),As).
As = [1, 1]
?- all evals(imp(a,b),As).
As = [1, 0, 1, 1]

*/

all_evals(X,As) :- vars(X,S), evs(S,Es), evals(X,Es,As).


/* 

taut/1

Definiti un predicat taut/1, astfel incat, pentru orice formul ̆ X, avem ca taut(X) este adevarat exact atunci cand X este tautologie.
Exemple:
?- taut(imp(a,a)).
true
?- taut(imp(a,b)).
false
10

*/

all_ones([]).
all_ones([1|T]) :- all_ones(T).
taut(X) :- all_evals(X,As), all_ones(As).

/*
C07-08 - Programare imperativa
*/

/* 
C09-10-11 - Lambda calcul .. corespondenta
*/