/*
Schemele relationale

FORMATIE ( id_formatie, nume, data_lansare, data_retragere, website, tara_prov )
PREMIU ( id_premiu, concurs, sectiune, frecventa, tara_prov )
CASTIGA ( id_premiu, id_formatie, data_d, loc_ocupat, recompensa )
ALBUM ( id_album, id_formatie, gen, nume, data_l, pret ) 

*/

CREATE TABLE FORMATIE (id_formatie NUMBER, nume VARCHAR(15), data_lansare DATE, data_retragere DATE, website VARCHAR(50), tara_prov VARCHAR(50));
CREATE TABLE PREMIU (id_premiu NUMBER, concurs VARCHAR(50), sectiune VARCHAR(50), frecventa NUMBER, tara_prov VARCHAR(50));
CREATE TABLE CASTIGA (id_premiu NUMBER, id_formatie NUMBER, data_d DATE, loc_ocupat NUMBER, recompensa NUMBER );
CREATE TABLE ALBUM ( id_album NUMBER, id_formatie NUMBER, gen VARCHAR(50), nume VARCHAR(50), data_l DATE, pret NUMBER);

INSERT INTO FORMATIE (id_formatie, nume, data_lansare, data_retragere, website, tara_prov)
VALUES (1,'GRock',TO_DATE('12-8-2025', 'DD-MM-YYYY'),SYSDATE,'GRock.com','Romania');
INSERT INTO FORMATIE (id_formatie, nume, data_lansare, data_retragere, website, tara_prov)
VALUES (2,'GSM',TO_DATE('12-6-1997', 'DD-MM-YYYY'),TO_DATE('12-1-2000', 'DD-MM-YYYY'),'GSM.com','Olanda');
INSERT INTO FORMATIE (id_formatie, nume, data_lansare, data_retragere, website, tara_prov)
VALUES (3,'Hippie',TO_DATE('11-2-1998', 'DD-MM-YYYY'),TO_DATE('2-1-2005', 'DD-MM-YYYY'),'Hippie.com','Polonia');
INSERT INTO FORMATIE (id_formatie, nume, data_lansare, data_retragere, website, tara_prov)
VALUES (4,'Armored Tank',TO_DATE('10-3-2010', 'DD-MM-YYYY'),TO_DATE('9-1-2009', 'DD-MM-YYYY'),'Arm.com','Germania');
INSERT INTO FORMATIE (id_formatie, nume, data_lansare, data_retragere, website, tara_prov)
VALUES (5,'Crown',TO_DATE('01-1-2000', 'DD-MM-YYYY'),TO_DATE('12-1-2010', 'DD-MM-YYYY'),'Crown.com','Germania');

SELECT * FROM FORMATIE;

INSERT INTO PREMIU (id_premiu, concurs, sectiune, frecventa, tara_prov)
VALUES ( 1, 'Euro', 'Rock', 120, 'Romania');
INSERT INTO PREMIU (id_premiu, concurs, sectiune, frecventa, tara_prov)
VALUES ( 2, 'Star', 'Hip-hop', 200, 'Franta');
INSERT INTO PREMIU (id_premiu, concurs, sectiune, frecventa, tara_prov)
VALUES ( 3, 'MostWanted', 'General', 12, 'Italia');

SELECT * FROM PREMIU;

INSERT INTO CASTIGA (id_premiu, id_formatie, data_d, loc_ocupat, recompensa)
VALUES (1,1,TO_DATE('12-8-2025','DD-MM-YYYY'),1,10000);
INSERT INTO CASTIGA (id_premiu, id_formatie, data_d, loc_ocupat, recompensa)
VALUES (2,5,TO_DATE('11-2-2009','DD-MM-YYYY'),1,100000);
INSERT INTO CASTIGA (id_premiu, id_formatie, data_d, loc_ocupat, recompensa)
VALUES (3,4,TO_DATE('11-2-2009','DD-MM-YYYY'),2,50000);
INSERT INTO CASTIGA (id_premiu, id_formatie, data_d, loc_ocupat, recompensa)
VALUES (4,2,TO_DATE('11-2-1998','DD-MM-YYYY'),3,1000);


SELECT * FROM CASTIGA;

INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (1,1,'Rock','Below the Sea',TO_DATE('12-8-2025', 'DD-MM-YYYY'), 100);
INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (2,2,'Rock','D13',TO_DATE('12-3-1998', 'DD-MM-YYYY'), 500);
INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (3,3,'Pop','D13',TO_DATE('12-3-1999', 'DD-MM-YYYY'), 500);
INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (4,4,'Rock','For the king',TO_DATE('11-3-2011', 'DD-MM-YYYY'), 1000);
INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (5,5,'Rock','Kings',TO_DATE('10-10-2002', 'DD-MM-YYYY'), 350);
INSERT INTO ALBUM ( id_album, id_formatie, gen, nume, data_l, pret)
VALUES (6,1,'Rock','Solace',TO_DATE('12-8-2025', 'DD-MM-YYYY'), 200);

SELECT * FROM ALBUM;

-- 1) Subprogram care primeste ca parametru un cod de formatie si returneaza lista albumelor lansate de aceasta, precum si lista premiilor castigate dupa fiecare album lansat. 
-- Testati prin intermediul unui bloc anonim = PL/SQL simplu

SET SERVEROUTPUT ON

SELECT * FROM ALBUM WHERE id_formatie = 1;

DECLARE
    type t_album IS VARRAY(50) OF album.nume%TYPE;
    v_cod formatie.id_formatie%TYPE := &cod;
    v_nume formatie.nume%TYPE;
    v_album t_album := t_album();
    v_premiu_id castiga.id_premiu%TYPE;
BEGIN
    SELECT nume INTO v_nume FROM FORMATIE WHERE id_formatie = v_cod;
--    SELECT nume INTO v_album FROM ALBUM WHERE id_formatie = v_cod;
    
    FOR premiu IN ( SELECT * FROM castiga ) LOOP
        IF premiu.id_formatie = v_cod THEN
            FOR contor IN ( SELECT * FROM premiu ) LOOP
                IF contor.id_premiu = premiu.id_premiu THEN
                    dbms_output.put_line('Lista cu premii pentru formatia ' || v_nume || ' este: ' || contor.concurs);
                END IF;
            END LOOP;
        END IF;
    END LOOP;

    -- lista albumelor + lista premiilor castigate dupa fiecare album lansat ( castiga.data_d > album.data_l )
    FOR album IN ( SELECT nume FROM album WHERE id_formatie = v_cod ) LOOP
        dbms_output.put_line('Lista cu albume pentru formatia ' || v_nume || ' este: ' || album.nume);
    END LOOP;
END;


CREATE OR REPLACE PROCEDURE afiseaza_albume_si_premii (
    p_cod IN formatie.id_formatie%TYPE
) IS
    v_nume formatie.nume%TYPE;
BEGIN
    SELECT nume INTO v_nume
    FROM formatie
    WHERE id_formatie = p_cod;

    -- Pentru fiecare album, listăm 0..n premii luate DUPĂ data lansării acelui album
    FOR r IN (
        SELECT
            a.nume        AS nume_album,
            a.data_l      AS data_album,
            p.concurs     AS nume_premiu,
            c.data_d      AS data_premiu
        FROM album a
        LEFT JOIN castiga c
               ON c.id_formatie = a.id_formatie
              AND c.data_d     > a.data_l            -- "după fiecare album"
        LEFT JOIN premiu p
               ON p.id_premiu = c.id_premiu
        WHERE a.id_formatie = p_cod
        ORDER BY a.data_l, c.data_d
    ) LOOP
        IF r.nume_premiu IS NULL THEN
            DBMS_OUTPUT.PUT_LINE(
                'Album: ' || r.nume_album ||
                ' (' || TO_CHAR(r.data_album, 'YYYY-MM-DD') || ') — fără premii'
            );
        ELSE
            DBMS_OUTPUT.PUT_LINE(
                'Album: ' || r.nume_album ||
                ' (' || TO_CHAR(r.data_album, 'YYYY-MM-DD') || ') — premiu: ' ||
                r.nume_premiu || ' (' || TO_CHAR(r.data_premiu, 'YYYY-MM-DD') || ')'
            );
        END IF;
    END LOOP;

EXCEPTION
    WHEN NO_DATA_FOUND THEN
        DBMS_OUTPUT.PUT_LINE('Formația cu cod ' || p_cod || ' nu există.');
END;
/

DECLARE
    v_cod formatie.id_formatie%TYPE := &cod;  -- te va întreba codul
BEGIN
    afiseaza_albume_si_premii(v_cod);
END;


