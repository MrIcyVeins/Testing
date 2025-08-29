SET SERVEROUTPUT ON;


-- CREAREA TABELELOR

DROP TABLE FORMATIE;
DROP TABLE ALBUM;
DROP TABLE PREMIU;
DROP TABLE CASTIGA;

CREATE TABLE FORMATIE (
    id_formatie   NUMBER PRIMARY KEY,
    nume          VARCHAR2(100) NOT NULL,
    data_lansare  DATE,
    data_retragere DATE,
    website       VARCHAR2(200),
    tara_prov     VARCHAR2(50)
);

CREATE TABLE PREMIU (
    id_premiu   NUMBER PRIMARY KEY,
    concurs     VARCHAR2(100) NOT NULL,
    sectiune    VARCHAR2(100),
    frecventa   VARCHAR2(50),
    tara_prov   VARCHAR2(50)
);

CREATE TABLE CASTIGA (
    id_premiu    NUMBER,
    id_formatie  NUMBER,
    data_d       DATE,
    loc_ocupat   NUMBER,
    recompensa   VARCHAR2(100),
    PRIMARY KEY (id_premiu, id_formatie, data_d),
    FOREIGN KEY (id_premiu) REFERENCES PREMIU(id_premiu),
    FOREIGN KEY (id_formatie) REFERENCES FORMATIE(id_formatie)
);

CREATE TABLE ALBUM (
    id_album     NUMBER PRIMARY KEY,
    id_formatie  NUMBER,
    gen          VARCHAR2(50),
    nume         VARCHAR2(100),
    data_l       DATE,
    pret         NUMBER(8,2),
    FOREIGN KEY (id_formatie) REFERENCES FORMATIE(id_formatie)
);

-- INSERARE VALORI DEMO

-- FORMATIE
INSERT INTO FORMATIE VALUES (1, 'Metallica', TO_DATE('1981-10-01','YYYY-MM-DD'), NULL, 'https://www.metallica.com', 'USA');
INSERT INTO FORMATIE VALUES (2, 'ABBA', TO_DATE('1972-01-01','YYYY-MM-DD'), TO_DATE('1982-12-31','YYYY-MM-DD'), 'https://www.abbasite.com', 'Sweden');
INSERT INTO FORMATIE VALUES (3, 'Phoenix', TO_DATE('1962-01-01','YYYY-MM-DD'), NULL, 'https://www.phoenix.ro', 'Romania');

-- PREMIU
INSERT INTO PREMIU VALUES (1, 'Grammy Awards', 'Best Metal Performance', 'Anual', 'USA');
INSERT INTO PREMIU VALUES (2, 'Eurovision', 'Best Song', 'Anual', 'Europe');
INSERT INTO PREMIU VALUES (3, 'MTV Music Awards', 'Best Band', 'Anual', 'USA');

-- CASTIGA
INSERT INTO CASTIGA VALUES (1, 1, TO_DATE('1992-02-20','YYYY-MM-DD'), 1, 'Trofeu + 10.000$');
INSERT INTO CASTIGA VALUES (2, 2, TO_DATE('1974-04-06','YYYY-MM-DD'), 1, 'Trofeu');
INSERT INTO CASTIGA VALUES (3, 3, TO_DATE('2010-11-07','YYYY-MM-DD'), 2, 'Diploma + promovare');

-- ALBUM
INSERT INTO ALBUM VALUES (1, 1, 'Metal', 'Master of Puppets', TO_DATE('1986-03-03','YYYY-MM-DD'), 19.99);
INSERT INTO ALBUM VALUES (2, 2, 'Pop', 'Waterloo', TO_DATE('1974-04-04','YYYY-MM-DD'), 15.50);
INSERT INTO ALBUM VALUES (3, 3, 'Rock', 'Mugur de fluier', TO_DATE('1974-01-01','YYYY-MM-DD'), 12.00);
INSERT INTO ALBUM VALUES (4, 1, 'Metal', 'Black Album', TO_DATE('1991-08-12','YYYY-MM-DD'), 21.50);


/*

FORMATIE ( id_formatie, nume, data_lansare, data_retragere, website, tara_prov );
PREMIU ( id_premiu, concurs, sectiune, frecventa, tara_prov );
CASTIGA ( id_premiu, id_formatie, data_d, loc_ocupat, recompensa);
ALBUM ( id_album, id_formatie, gen, nume, data_l, pret);

1. Subprogram care primeste ca parametru un cod de formatie si returneaza lista albumelor lansate de aceasta, precum si lista premiilor castigate dupa fiecre album lansat
Testati prin intermediul unui bloc anonim

*/

SET SERVEROUTPUT ON;
SELECT * FROM PREMIU;
SELECT * FROM CASTIGA;
SELECT * FROM ALBUM;
DESC ALBUM;
DESC CASTIGA;


CREATE OR REPLACE TYPE formatie_obiect AS OBJECT(
    nume_album VARCHAR2(100),
    recompensa_castigata VARCHAR2(100)
);

CREATE OR REPLACE TYPE t_tabel AS TABLE OF formatie_obiect; 

CREATE OR REPLACE FUNCTION formatie_functie(p_cod IN formatie.id_formatie%TYPE)
    RETURN t_tabel
IS
    v_tab t_tabel := t_tabel();
BEGIN
    SELECT formatie_obiect(a.nume, c.recompensa)
    BULK COLLECT INTO v_tab
    FROM formatie f
    LEFT JOIN album a ON a.id_formatie = f.id_formatie
    LEFT JOIN castiga c ON c.id_formatie = f.id_formatie
                        AND c.data_d > a.data_l
    WHERE f.id_formatie = p_cod;
    RETURN v_tab;
END;


DECLARE
    id_formatie NUMBER := 1;
BEGIN
    FOR r IN ( SELECT * FROM TABLE(formatie_functie(id_formatie))) LOOP
        DBMS_OUTPUT.PUT_LINE('Album' || r.nume_album || ' ' || 'Recompensa: ' || r.recompensa_castigata);
    END LOOP;
END;


-- testare cu bloc anonim ( PL/SQL ) 
DECLARE
    v_chemare NUMBER;
BEGIN
    v_chemare := formatie_functie(1);
    DBMS_OUTPUT.PUT_LINE('Rezultatul functiei: ' || v_chemare);
END;


/*

2. Definiti un tabel care sa contina o coloana de tip obiect si o coloana de tip tablou. Folositi un subprogram care sa populeze tabelul definit anterior cu cel putin 3 linii

*/

CREATE OR REPLACE TYPE obiect_ex2 AS OBJECT(
    ceva NUMBER,
    altceva VARCHAR2(10)
);

CREATE OR REPLACE TYPE tablou_ex2 AS TABLE OF obiect_ex2;

CREATE TABLE tabel_ex2(coloana_obiect obiect_ex2, coloana_tablou tablou_ex2) NESTED TABLE coloana_tablou STORE AS coloana_tablou_nested_table;

BEGIN
    INSERT INTO tabel_ex2(coloana_obiect, coloana_tablou)
    VALUES(obiect_ex2(1,'valoare o'), tablou_ex2(obiect_ex2(1,'valoare t')));
END;

SELECT * FROM tabel_ex2;

/*

3. DEfiniti un tip de date stocat care sa va permita sa salvati continutul ALBUM. Afisati continutul unei variabile de acest tip dupa initializarea acesteia cu infromatiile
care e regasesc in tabelul album

*/

SELECT * FROM ALBUM;
DESC ALBUM;

CREATE OR REPLACE TYPE album_obiect AS OBJECT(
        id_a NUMBER,
        id_formatie NUMBER,
        gen VARCHAR2(50),
        nume VARCHAR2(100),
        data_l DATE,
        pret    NUMBER(8,2)
);

CREATE OR REPLACE TYPE album_tablou AS TABLE OF album_obiect;
DROP TYPE album_tablou;

DECLARE 
    v_album album_tablou;
BEGIN
    SELECT album_obiect(id_album, id_formatie, gen, nume, data_l, pret) -- trebuie sa fie la fel ca in tabelul original ( coloanele din tabelul original ) 
    BULK COLLECT INTO v_album
    FROM album;
--    ORDER BY data_l NULLS LAST, id_a;
    
    FOR i in 1 .. v_album.COUNT LOOP
        DBMS_OUTPUT.PUT_LINE( v_album(i).id_album || ' ' || v_album(i).id_formatie ); -- trebuie sa fie ca in obiect ( coloanele din tabel ) 
    END LOOP;
END;


/*
PREZENTARE(cod_pr, data, oras, nume)
SPONSOR(cod_sponsor, nume, info, tara_origine)
SUSTINE(cod_pr, cod_sp, suma)
VESTIMENTATIE(cod_vestimentatie, denumire, valoare, cod_prezentare)

Subprogram prin care sa se obtina media valorilor oferite de un sponsor al carui cod este introdus ca parametru, pentru prezentarile in care au existat minim x vestimentatii
Parametrul x va avea valoare implicita
Sa se apelee subprogramul

*/

SELECT * FROM PREZENTARE;
SELECT * FROM VESTIMENTATIE;
SELECT * FROM SUSTINE;

WHERE cod_sponsor = 1 AND count(cod_vestimentatie) 

SELECT 
    s.cod_sponsor,
    avg(vs.valoare) as medie_valori,
    vs.cod_prezentare,
    count(vs.cod_vestimentatie) as numar_vestimentatii
FROM sponsor s 
LEFT JOIN sustine ss ON ss.cod_sp = s.cod_sponsor
LEFT JOIN vestimentatie vs ON vs.cod_prezentare = ss.cod_pr
WHERE s.cod_sponsor = 3
GROUP BY s.cod_sponsor, vs.cod_prezentare
HAVING COUNT(vs.cod_vestimentatie) >= 2;  -- pentru count se pune HAVING COUNT 


CREATE OR REPLACE TYPE vest_obiect AS OBJECT(
    cod_sp NUMBER,
    medie NUMBER,
    cod_pr NUMBER,
    numar_vest NUMBER    
);
CREATE OR REPLACE TYPE vest_tablou AS TABLE OF vest_obiect;
DROP TYPE vest_tablou;

CREATE OR REPLACE PROCEDURE p_medie (p_cod IN sponsor.cod_sponsor%TYPE, p_minim_vest IN NUMBER DEFAULT 2) -- variabila implicita 2
IS
--    v_minim_vest NUMBER := 2; -- variabila locala 
    v_rezultat vest_tablou := vest_tablou();
BEGIN
    SELECT vest_obiect(              -- fara as in blocurile subprogramelor
        s.cod_sponsor,
        avg(vs.valoare),
        vs.cod_prezentare,
        count(vs.cod_vestimentatie)
      )
    BULK COLLECT INTO v_rezultat
    FROM sponsor s 
    LEFT JOIN sustine ss ON ss.cod_sp = s.cod_sponsor
    LEFT JOIN vestimentatie vs ON vs.cod_prezentare = ss.cod_pr
    WHERE s.cod_sponsor = p_cod
    GROUP BY s.cod_sponsor, vs.cod_prezentare
    HAVING COUNT(vs.cod_vestimentatie) >= p_minim_vest;  -- pentru count se pune HAVING COUNT 
    
    FOR i in 1 .. v_rezultat.COUNT LOOP
        dbms_output.put_line('Media valorilor oferite de catre sponsorul ' || v_rezultat(i).numar_vest || ' este ' || v_rezultat(i).medie);
    END LOOP;
END;

exec p_medie(1);


/*

Adauati constragerea de cheie externa dintre tabelele PREZENTARE SI VESTIMENTATIE. Implementati comportamentil ON DELETE CASCADE cu ajutorul unui trigger. Testati trigger-ul

*/

-- constrangerea de cheie externa
-- explicatie: la stergerea unui rand din prezentare, triggerul sterge intai toate randurile copil din vestimentatie cu acelasi cod_prezentare
-- astfel stergerea parintelui nu mai incalca FK si se comporta ca un DELETE CASCADE 

ALTER TABLE VESTIMENTATIE
    ADD CONSTRAINT fk_vestimentatie_prezentare
    FOREIGN KEY (cod_prezentare)
    REFERENCES PREZENTARE(cod_pr);
    
CREATE OR REPLACE TRIGGER trg_prezentare_del_cascade
BEFORE DELETE ON PREZENTARE
FOR EACH ROW
BEGIN
    DELETE FROM VESTIMENTATIE
    WHERE cod_prezentare = :OLD.cod_pr;
END;
    
INSERT INTO PREZENTARE (cod_pr, data, oras, nume)
VALUES (100, DATE '2024-05-10', 'Paris', 'Test Show');

INSERT INTO VESTIMENTATIE (cod_vestimentatie, denumire, valoare, cod_prezentare)
VALUES (1001, 'Rochie test', 12000, 100);

INSERT INTO VESTIMENTATIE (cod_vestimentatie, denumire, valoare, cod_prezentare)
VALUES (1002, 'Sacou test', 8000, 100);

COMMIT;

SELECT * FROM PREZENTARE     WHERE cod_pr = 100;
SELECT * FROM VESTIMENTATIE  WHERE cod_prezentare = 100;

DELETE FROM PREZENTARE WHERE cod_pr = 100;
COMMIT;

SELECT * FROM PREZENTARE     WHERE cod_pr = 100; 
SELECT * FROM VESTIMENTATIE  WHERE cod_prezentare = 100;


