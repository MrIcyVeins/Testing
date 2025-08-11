--drop table employees cascade constraints;
--drop table departments cascade constraints;
--drop table jobs cascade constraints;
--drop table locations cascade constraints;
--drop table countries cascade constraints;
--drop table regions cascade constraints;
--drop table job_history cascade constraints;
--drop table works_on cascade constraints;
--drop table project cascade constraints;
--drop table projects cascade constraints;
--drop table JOB_GRADES cascade constraints;

SELECT * FROM job_history;
SELECT * FROM employees;
SELECT * FROM departments;
SELECT * FROM locations;
SELECT * FROM countries;
SELECT * FROM emp_details_view; -- joins employees + jobs

/*
I.Exercitii simple

1) Pentru fiecare oras sa se afiseze numele tarii in care se afla si numarul de angajati din cadrul sau.
2) Sa se afiseze numele, salariul, titlul job-ului, orasul si tara in care lucreaza angajatii condusi direct de King.
3) Afisati codul, numele si salariul tuturor angajatilor care castiga mai mult decat salariu mediu pentru job-ul corespunzator 
lucreaza intr-un departament cu cel putin unul din angajati al caror nume contine litera t.
4) Afisati angajatii care au ocupat de mai multe ori aceeasi functie.
5) Sa se afiseze salariatii care au fost angajati in aceeasi zi a lunii in care cei mai multi dintre salariati au fost angajati.
*/

SELECT * FROM EMPLOYEES;
SELECT * FROM COUNTRIES;
SELECT * FROM JOBS;

--1)
select loc.city, ctr.country_name, count(emp.employee_id)
from locations loc
left join countries ctr on loc.country_id = ctr.country_id
join departments dept on loc.location_id = dept.location_id
join employees emp on dept.department_id = emp.department_id
group by loc.city, ctr.country_name;

--2)
SELECT 
  e.first_name || ' ' || e.last_name AS employee,
  e.salary,
  j.job_title,
  l.city,
  c.country_name,
  m.first_name || ' ' || m.last_name AS manager
FROM employees   e
JOIN employees   m ON e.manager_id     = m.employee_id
LEFT JOIN jobs        j ON e.job_id        = j.job_id
LEFT JOIN departments d ON e.department_id = d.department_id
LEFT JOIN locations   l ON d.location_id   = l.location_id
LEFT JOIN countries   c ON l.country_id    = c.country_id
WHERE m.last_name = 'King'
ORDER BY employee;

--3)
SELECT 
  e.employee_id AS cod,
  e.first_name || ' ' || e.last_name AS nume,
  e.salary
FROM employees e
WHERE e.salary > (
  SELECT AVG(e2.salary)
  FROM employees e2
  WHERE e2.job_id = e.job_id             -- media pe job-ul angajatului curent
)
AND EXISTS (
  SELECT 1
  FROM employees x
  WHERE x.department_id = e.department_id -- acela?i departament
    AND (
      LOWER(x.first_name) LIKE '%t%' 
      OR LOWER(x.last_name)  LIKE '%t%'   -- numele con?ine litera „t” (case-insensitive)
    )
)
ORDER BY cod;

--4)
SELECT employee_id, count(employee_Id)
from job_history
group by employee_id, job_id
having count(employee_id) > 1;

-- mai detaliat
SELECT employee_id, job_id, COUNT(*) AS nr_ocupari
FROM job_history
GROUP BY employee_id, job_id
HAVING COUNT(*) > 1;

--5) 
SELECT e.employee_id,
       e.first_name || ' ' || e.last_name AS nume,
       e.hire_date,
       e.salary
FROM employees e
JOIN (
  SELECT EXTRACT(DAY FROM hire_date) AS dday,
         RANK() OVER (ORDER BY COUNT(*) DESC) AS rnk
  FROM employees
  GROUP BY EXTRACT(DAY FROM hire_date)
) t
  ON t.dday = EXTRACT(DAY FROM e.hire_date)
WHERE t.rnk = 1
ORDER BY e.hire_date, e.employee_id;

-- JOIN si AGREGARI
--6) Pentru fiecare ?ar?, afi?a?i num?rul de ora?e (locations) ?i num?rul de departamente din acea ?ar?.

SELECT 
    DISTINCT cou.country_name,
    COUNT(DISTINCT loc.location_id) as numar_orase,
    COUNT(DISTINCT dep.department_id) as numar_departamente
FROM locations loc
LEFT JOIN departments dep on dep.location_id = loc.location_id
LEFT JOIN countries cou on cou.country_id = loc.country_id
GROUP BY cou.country_name
ORDER BY numar_orase DESC;


--7) Pentru fiecare departament, afi?a?i salariul mediu, salariul minim ?i num?rul de angaja?i; ordona?i descresc?tor dup? salariul mediu.

SELECT
    emp.department_id,
    min(emp.salary) AS salariu_minim,
    ROUND(avg(emp.salary), 2) AS salariu_mediu, -- round optional pentru 2 zecimale
    count(emp.employee_id) AS numar_angajati
FROM employees emp
WHERE emp.department_id IS NOT NULL -- optional omite departamentul null
GROUP BY emp.department_id
ORDER BY salariu_mediu DESC;

--8) Afisati **top 3 orase** cu cei mai multi angajati (daca egalitati, toate la pozitia 3).

employees > departments > locations - city

-- varianta 1
WITH orase AS (
    SELECT 
        loc.city,
        COUNT(emp.employee_id) AS numar_angajati
    FROM departments dep
    LEFT JOIN locations loc 
           ON dep.location_id = loc.location_id
    LEFT JOIN employees emp 
           ON emp.department_id = dep.department_id
    GROUP BY loc.city
)
SELECT 
    city,
    numar_angajati,
    pozitie
FROM (
  SELECT
    city,
    numar_angajati,
    RANK() OVER (ORDER BY numar_angajati DESC) AS pozitie
  FROM orase
)
WHERE pozitie <= 3
ORDER BY pozitie, city ASC;

-- varianta cu fetch first <number> rows with ties
SELECT city, numar_angajati
FROM (
  SELECT
    loc.city AS city,
    COUNT(e.employee_id) AS numar_angajati
  FROM employees e
  JOIN departments d ON d.department_id = e.department_id
  JOIN locations  loc ON loc.location_id = d.location_id
  GROUP BY loc.city
)
ORDER BY numar_angajati DESC
FETCH FIRST 3 ROWS WITH TIES; -- primele 3 linii + egalitatile

--9) Afi?a?i **angaja?ii** împreun? cu **numele managerului** (self-join pe `employees`). Include?i ?i pe cei f?r? manager (manager = `NULL`).

SELECT 
    emp.employee_id,
    emp.first_name AS nume_angajat,
    emp1.first_name AS nume_manager
FROM employees emp
LEFT JOIN employees emp1 on emp.manager_id = emp1.employee_id
ORDER BY nume_manager NULLS FIRST, nume_angajat;

--10) Afi?a?i angaja?ii care câ?tig? **peste media pe departamentul lor**.

SELECT 
    emp.employee_id,
    emp.department_id,
    emp.salary
FROM employees emp
WHERE emp.salary > (
    SELECT avg(salary)
    FROM employees
    WHERE department_id = emp.department_id
)
ORDER BY emp.department_id, emp.salary;

--11) 6. Afi?a?i **departamentele f?r? angaja?i** (exist? în `departments`, dar nu apar în `employees`).

SELECT * FROM DEPARTMENTS;
SELECT * FROM EMPLOYEES;

-- varianta 1 ( LEFT JOIN + IS NULL )
SELECT d.department_id, d.department_name
FROM departments d
LEFT JOIN employees e
       ON e.department_id = d.department_id
WHERE e.department_id IS NULL;

-- varianta 2 ( NOT EXISTS )
SELECT d.department_id, d.department_name
FROM departments d
WHERE NOT EXISTS (
  SELECT 1
  FROM employees e
  WHERE e.department_id = d.department_id
);

-- varianta 3 ( HAVING ) 
SELECT d.department_id, d.department_name
FROM departments d
LEFT JOIN employees e
       ON e.department_id = d.department_id
GROUP BY d.department_id, d.department_name
HAVING COUNT(e.employee_id) = 0;

-- 12) Afi?a?i angaja?ii care **nu** au avut **niciodat?** în `job_history` un job diferit de cel curent.

SELECT * FROM JOB_HISTORY;

SELECT emp.employee_id, emp.first_name, emp.last_name
FROM employees emp
WHERE NOT EXISTS (
  SELECT 1
  FROM job_history jb
  WHERE jb.employee_id = emp.employee_id
    AND jb.job_id <> emp.job_id
);

-- 13) Afi?a?i **angaja?ii** care lucreaz? în **aceea?i ?ar?** cu **managerul lor**.

SELECT * FROM EMPLOYEES;
SELECT * FROM DEPARTMENTS;
SELECT * FROM countries;
SELECT * FROM locations;

SELECT emp.employee_id, emp.first_name, emp.last_name, man_cou.country_name, man_emp.first_name as MANAGER_NAME
FROM employees emp
JOIN employees man_emp on man_emp.employee_id = emp.manager_id 
JOIN departments emp_dep on emp_dep.department_id = emp.department_id
JOIN locations emp_loc on emp_loc.location_id = emp_dep.location_id
JOIN countries emp_cou ON emp_cou.country_id = emp_loc.country_id
JOIN departments man_dep on man_dep.department_id = man_emp.department_id
JOIN locations man_loc on man_loc.location_id = man_dep.location_id
JOIN countries man_cou on man_cou.country_id = man_loc.country_id
WHERE emp_cou.country_id = man_cou.country_id;

--13) Pentru fiecare **lun?** (1..12), afi?a?i **câ?i angaja?i** au fost angaja?i în acea lun? (indiferent de an).
SELECT 
    EXTRACT(MONTH FROM hire_date) as luna,
    COUNT(*) as numar_angajati
FROM employees
GROUP BY EXTRACT(MONTH FROM hire_date)
ORDER BY luna DESC;

--14) Afi?a?i angaja?ii al c?ror **email** are exact **5 litere** înainte de `@` (dac? p?stra?i conven?ia `email` f?r? domeniu, adapta?i cerin?a: selecta?i emailuri de **5 caractere**).

SELECT employee_id,first_name,email from employees
WHERE length(email) = 5;


--15) Afi?a?i angaja?ii angaja?i într-o **zi de luni**, cu `hire_date` ?i `TO_CHAR(hire_date, 'Day')`.
-- NLS_DATE_LANGUAGE=ROMANIAN - pentru date

SELECT 
    employee_id,
    TO_CHAR(hire_date, 'FMDAY', 'NLS_DATE_LANGUAGE=ROMANIAN') AS zi
FROM employees
WHERE TO_CHAR(hire_date, 'FMDAY', 'NLS_DATE_LANGUAGE=ROMANIAN') = 'LUNI';

--16) Pentru fiecare **job**, marca?i angaja?ii cu un flag “**Peste medie**/Nu\*\*” în func?ie de media pe job (folosi?i `AVG(...) OVER (PARTITION BY job_id)`).

SELECT
  e.employee_id,
  e.first_name || ' ' || e.last_name AS nume,
  e.job_id,
  j.job_title,
  e.salary,
  ROUND(avg_job, 2)                  AS salariu_mediu_job,
  CASE
    WHEN e.salary > avg_job THEN 'Peste medie'
    ELSE 'Nu'
  END                                AS flag
FROM (
  SELECT emp.employee_id,
         emp.first_name,
         emp.last_name,
         emp.job_id,
         emp.salary,
         AVG(emp.salary) OVER (PARTITION BY emp.job_id) AS avg_job
  FROM employees emp
) e
LEFT JOIN jobs j ON j.job_id = e.job_id
ORDER BY e.job_id, e.salary DESC;

--17) Afi?a?i, pentru fiecare **departament**, **top 2 salarii** (dac? egalit??i la locul 2, include?i-le pe toate) – `DENSE_RANK()`. (*)

SELECT emp.department_id, emp.salary, emp.employee_id
FROM (
    SELECT
        e.department_id,
        e.employee_id,
        e.salary,
        DENSE_RANK() OVER (
          PARTITION BY e.department_id
          ORDER BY e.salary DESC NULLS LAST
        ) AS rnk
    FROM employees e
) emp
WHERE emp.rnk  <= 2 
ORDER BY emp.department_id, emp.salary DESC, emp.employee_id;

--18) Afi?a?i angaja?ii care au avut în `job_history` un **job din alt? familie** decât cel curent (de ex. `SA%` vs `IT%`), presupunând prefixe semnificative în `job_id`.

SELECT e.employee_id,
       e.first_name || ' ' || e.last_name AS nume,
       e.job_id AS job_curent
FROM employees e
WHERE EXISTS (
  SELECT 1
  FROM job_history jh
  WHERE jh.employee_id = e.employee_id
    AND SUBSTR(e.job_id, 1, 2) <> SUBSTR(jh.job_id, 1, 2)
);

--19) Afi?a?i angaja?ii care au revenit la **acela?i job** dup? ce au avut între timp alt job (pattern „A ? B ? A”).
WITH jh AS (
  SELECT
    employee_id,
    job_id,
    start_date,
    end_date,
    ROW_NUMBER() OVER (PARTITION BY employee_id ORDER BY end_date DESC) AS rn
  FROM job_history
)
SELECT
  e.employee_id,
  e.first_name || ' ' || e.last_name AS nume,
  e.job_id AS job_curent
FROM employees e
JOIN jh j_last
  ON j_last.employee_id = e.employee_id
 AND j_last.rn = 1                           -- ultimul job din istoric (imediat anterior celui curent)
WHERE j_last.job_id <> e.job_id              -- B: ultimul istoric e diferit de jobul curent (A)
  AND EXISTS (                               -- exist? un A în istoric
        SELECT 1
        FROM job_history j_prev
        WHERE j_prev.employee_id = e.employee_id
          AND j_prev.job_id = e.job_id
      )
ORDER BY e.employee_id;

-- 20) Pentru fiecare **proiect**, afi?a?i **num?rul de angaja?i** aloca?i ?i **num?rul total de zile** lucrate (suma `end_date - start_date`).

SELECT 
  p.project_id,
  p.project_name,
  COUNT(DISTINCT w.employee_id)                 AS numar_angajati,
  SUM( NVL(w.end_date, SYSDATE) - w.start_date) AS total_zile_lucrate
FROM projects  p
LEFT JOIN works_on w
       ON w.project_id = p.project_id
GROUP BY p.project_id, p.project_name
ORDER BY p.project_id;

--21) Afi?a?i **angaja?ii** care au lucrat pe **cel pu?in dou? proiecte diferite**.
--versiunea1
SELECT w.employee_id 
FROM works_on w
JOIN employees e on e.employee_id = w.employee_id
WHERE e.hire_date < w.end_date
GROUP by w.employee_id

--versiunea2 ( HAVING COUNT DISTINCT = pentru valori diferite in acelasi tabel ) 
SELECT w.employee_id
FROM works_on w
GROUP BY w.employee_id
HAVING COUNT(DISTINCT w.project_id) >= 2;

--22) Afi?a?i **managerii de proiect** (`projects.project_manager`) care **nu** sunt aloca?i **ca resurs?** în `works_on` pe propriul lor proiect.

SELECT DISTINCT p.project_manager
FROM projects p
WHERE NOT EXISTS (
    SELECT 1
    FROM works_on w
    WHERE w.employee_id = p.project_manager
        AND w.project_id = p.project_id
);

--23) Afi?a?i **proiectele întârziate** (unde `delivery_date` > `deadline`) ?i **num?rul de zile** de întârziere.

-- cu SUM si group by
SELECT 
    p.project_name,
    SUM(p.delivery_date - p.deadline) AS zile_intarziate
FROM projects p 
WHERE p.delivery_date > p.deadline
GROUP BY p.project_name

-- fara si cu order by
SELECT 
    p.project_name,
    p.delivery_date - p.deadline AS zile_intarziate
FROM projects p 
WHERE p.delivery_date > p.deadline
ORDER BY zile_intarziate

--24) Afi?a?i **angajatii** care au lucrat pe **proiecte** în **cel putin doua luni calendaristice diferite** (pe baza intervalelor din `works_on`).

SELECT 
  e.employee_id,
  e.first_name || ' ' || e.last_name AS nume
FROM employees e
JOIN works_on w ON w.employee_id = e.employee_id
GROUP BY e.employee_id, e.first_name, e.last_name
HAVING MIN(TRUNC(w.start_date, 'MM')) 
    <> MAX(TRUNC(NVL(w.end_date, w.start_date), 'MM'))
ORDER BY e.employee_id;


--25)  Afișați **top 3 țări** cu cele mai multe **angajări** (număr de angajați), apoi – pentru fiecare astfel de țară – afișați **departamentul** cu **cel mai mare salariu mediu**.

--SELECT * FROM countries;
--SELECT * FROM locations;
--SELECT * FROM departments;
--SELECT * FROM employees;

-- v1
SELECT 
    count(emp.employee_id) AS numar_angajati,
    cou.country_id,
    cou.country_name,
    round(avg(emp.salary), 2) AS salariu_mediu
FROM employees emp
JOIN departments dep on dep.department_id = emp.department_id
JOIN locations loc on loc.location_id = dep.location_id
JOIN countries cou on cou.country_id = loc.country_id
GROUP BY cou.country_id, cou.country_name
ORDER BY salariu_mediu
FETCH FIRST 3 ROWS WITH TIES -- primele 3 


-- v2
WITH country_counts AS (                         -- nr. angajați pe țară
  SELECT c.country_id, c.country_name,
         COUNT(e.employee_id) AS numar_angajati
  FROM employees e
  JOIN departments d ON d.department_id = e.department_id
  JOIN locations  l ON l.location_id   = d.location_id
  JOIN countries  c ON c.country_id    = l.country_id
  GROUP BY c.country_id, c.country_name
),
top_countries AS (                               -- top 3 țări (cu ties)
  SELECT *
  FROM country_counts
  ORDER BY numar_angajati DESC
  FETCH FIRST 3 ROWS WITH TIES
),
dept_avg AS (                                    -- media pe departament, pe țară
  SELECT c.country_id, c.country_name,
         d.department_id, d.department_name,
         AVG(e.salary) AS salariu_mediu
  FROM employees e
  JOIN departments d ON d.department_id = e.department_id
  JOIN locations  l  ON l.location_id   = d.location_id
  JOIN countries  c  ON c.country_id    = l.country_id
  GROUP BY c.country_id, c.country_name,
           d.department_id, d.department_name
),
dept_rank AS (                                   -- departamentul cu media maximă, pe țară
  SELECT da.*,
         DENSE_RANK() OVER (
           PARTITION BY da.country_id
           ORDER BY da.salariu_mediu DESC
         ) AS rnk
  FROM dept_avg da
)
SELECT
  tc.country_name,
  tc.numar_angajati,
  dr.department_id,
  dr.department_name,
  ROUND(dr.salariu_mediu, 2) AS salariu_mediu
FROM top_countries tc
JOIN dept_rank dr
  ON dr.country_id = tc.country_id
WHERE dr.rnk = 1
ORDER BY tc.numar_angajati DESC, tc.country_name, dr.department_name;

--v3

SELECT
  tc.country_name,
  tc.numar_angajati,
  bd.department_id,
  bd.department_name,
  ROUND(bd.avg_salary, 2) AS salariu_mediu
FROM (
  -- top 3 țări după numărul de angajați (cu ties)
  SELECT c.country_id, c.country_name,
         COUNT(e.employee_id) AS numar_angajati
  FROM employees e
  JOIN departments d ON d.department_id = e.department_id
  JOIN locations  l ON l.location_id   = d.location_id
  JOIN countries  c ON c.country_id    = l.country_id
  GROUP BY c.country_id, c.country_name
  ORDER BY COUNT(e.employee_id) DESC
  FETCH FIRST 3 ROWS WITH TIES
) tc
CROSS APPLY (
  -- departamentul(ele) cu cea mai mare medie în țara respectivă (cu ties)
  SELECT d.department_id, d.department_name,
         AVG(e.salary) AS avg_salary
  FROM employees e
  JOIN departments d ON d.department_id = e.department_id
  JOIN locations  l  ON l.location_id   = d.location_id
  WHERE l.country_id = tc.country_id
  GROUP BY d.department_id, d.department_name
  ORDER BY AVG(e.salary) DESC
  FETCH FIRST 1 ROW WITH TIES
) bd
ORDER BY tc.numar_angajati DESC, tc.country_name, bd.department_name;

--26) Pentru fiecare **job**, afișați **distribuția pe intervale** în `JOB_GRADES` (în ce **grade** cade salariul fiecărui angajat); numărați câți angajați are fiecare job în fiecare grade.

SELECT * FROM JOBS;
SELECT * FROM EMPLOYEES;
SELECT * FROM job_grades;

-- fara tabel job_grades ( setare manuala )
SELECT 
    jb.job_id,
    e.employee_id,
    CASE
      WHEN e.salary > jb.min_salary AND e.salary < jb.min_salary + (jb.max_salary - jb.min_salary) THEN 'Grad min'
      WHEN e.salary = jb.min_salary + (jb.max_salary - jb.min_salary)   THEN 'Grad Mid'
      WHEN e.salary < jb.max_salary AND e.salary >= jb.min_salary + (jb.max_salary - jb.min_salary) THEN 'Grad Mare'
      ELSE 'Nu a fost gasit'
    END                                AS job_grade
FROM employees e
JOIN jobs jb on jb.job_id = e.job_id
ORDER BY e.employee_id, jb.job_id

-- cu tabel job_grades

SELECT 
    jb.job_id,
    jb.job_title,
    g.grade_level,
    COUNT(*) AS nr_angajati
FROM employees e
JOIN jobs jb on jb.job_id = e.job_id
JOIN job_grades g on e.salary BETWEEN g.lowest_sal AND g.highest_sal
GROUP BY jb.job_id, jb.job_title, g.grade_level
ORDER BY jb.job_id, g.grade_level

--27) Pentru fiecare **țară**, afișați **numărul de orașe** (locations) și **numărul de departamente** din acea țară.

SELECT * FROM countries;
SELECT * FROM locations;
SELECT * FROM departments;

SELECT
    loc.country_id,
    count(loc.location_id) as numar_orase,
    count(dep.department_id) as numar_departamente
FROM locations loc
LEFT JOIN departments dep on dep.location_id = loc.location_id 
GROUP BY loc.country_id
ORDER BY numar_departamente

/*
--1) Pentru fiecare oras sa se afiseze numele tarii in care se afla si numarul de angajati din cadrul sau.

select loc.city, ctr.country_name, count(emp.employee_id)
from locations loc
left join countries ctr on loc.country_id = ctr.country_id
join departments dept on loc.location_id = dept.location_id
join employees emp on dept.department_id = emp.department_id
group by loc.city, ctr.country_name;

--2) Sa se afiseze numele, salariul, titlul job-ului, orasul si tara in care lucreaza angajatii condusi direct de King.

select (e.first_name || ' ' || e.last_name) as employee, e.salary, j.job_title, 
    l.city, c.country_name, (m.first_name || ' ' || m.last_name) as manager
from employees e
join employees m on e.manager_id = m.employee_id
left join jobs j on e.job_id = j.job_id
left join departments d on e.department_id = d.department_id
left join locations l on d.location_id = l.location_id
left join countries c on l.country_id = c.country_id
where m.last_name = 'King';

--3) Afisati codul, numele si salariul tuturor angajatilor care castiga mai mult decat salariu mediu pentru job-ul corespunzator si lucreaza intr-un departament cu cel putin unul din angajati al caror nume contine litera „t”.

with in_same_department as (
    select distinct e.employee_id 
    from employees e
    join employees emp on e.department_id = emp.department_id
                and (emp.first_name like '%t%' or emp.last_name like '%t%')
                and e.employee_id <> emp.employee_id
    join departments d on d.department_id = e.department_id
)
select e.employee_id, concat(concat(e.first_name,' '), e.last_name) as name, e.salary, e.job_id
from employees e
where salary > (select avg(emp.salary)
                from employees emp 
                where emp.job_id = e.job_id)
      and e.employee_id in (select employee_id from in_same_department)
order by e.employee_id;

--4) Afisati angajatii care au ocupat de mai multe ori aceeasi functie.

select employee_id, count(employee_id) 
from job_history
group by employee_id, job_id
having count(employee_id) > 1;

--5) Sa se afiseze salariatii care au fost angajati in aceeasi zi a lunii in care cei mai multi dintre salariati au fost angajati.

with day_with_most_hires as (
    select extract(day from hire_date) as dday
    from employees
    group by extract(day from hire_date)
    having count(employee_id) = (
        select max(cnt) 
        from (
            select extract(day from hire_date), count(employee_id) as cnt
            from employees
            group by extract(day from hire_date)
            order by count(employee_id) desc
        )
    )
)
select employee_id
from employees
where extract(day from hire_date) = (select dday from day_with_most_hires);
*/


-- II. Exercitii functii/proceduri
/*
Exercitii functii/proceduri
1) Sa se scrie o procedura care sa intoarca angajatii (cod, nume, job_id)  care au avut cele mai multe joburi. Apelati aceasta procedura.
2) Sa se scrie o functie care primeste ca argument un cod de manager si intoarce numarul subalternilor acestuia. Apelati functia.
3) Sa se scrie o procedura care primeste ca argument un cod de angajat si intoarce, folosind acelasi parametru, codul angajatului care a fost angajat dupa acesta (se presupune ca este unic). Tratati erorile care pot sa apara. Apelati procedura.
4) Sa se adauge o colona (next_sef) tabelului emp (stocheaza viitorii sefi). Sa se scrie o "functie" care actualizeaza aceasta coloana. Viitorul sef va fi ales dintre subalternii (directi) sefului curent (angajatul care are vechimea cea mai mare dintre acestia). Comentati!
5) Se cere un subprogram care primeste ca argument 2 date si intoarce numarul de angajati care au lucrat la un proiect in aceasta perioada. Comentati care sunt modificarile necesare daca se cere lista angajatilor. Testati subprogramul.
6) O procedura care mareste cu p% salariul angajatilor care lucreaza in departamente care au minim 5 angajati si micsoreaza cu acelasi procent la ceilalti angajati. Sa se intoarca folosind un parametru numarul angajatilor care au obtinut marire de salariu, respectiv numarul de angajati carora li s-a micsorat salariul. Sa se apeleze procedura.
*/

-- III. Alte probleme
/*
Sa se afiseze codul, numele departamentului si numarul de angajati care lucreaza in acel departament pentru:
a) departamentele in care lucreaza mai putin de 4 angajati;
b) departamentul care are numarul maxim de angajati.
S? se creeze o cerere prin care s? se afi?eze num?rul total de angaja?i ?i, din acest total, num?rul celor care au fost angaja?i în 1997, 1998, 1999 si 2000. Denumiti capetele de tabel in mod corespunz?tor.
Pentru fiecare angajat s? se determine num?rul de subalterni (nu doar cei direc?i).
a) S? se ob?in? numele angaja?ilor care au lucrat cel pu?in pe acelea?i proiecte ca ?i angajatul având codul 200.
b) S? se ob?in? numele angaja?ilor care au lucrat cel mult pe acelea?i proiecte ca ?i angajatul având codul 200.
c) S? se ob?in? angaja?ii care au lucrat pe acelea?i proiecte ca ?i angajatul având codul 200.
*/

-- IV. Extra
/*
Probleme 2
 
Sa se afiseze codul, numele departamentului si numarul de angajati care lucreaza in acel departament pentru:
a) departamentele in care lucreaza mai putin de 4 angajati;
*************
select dep.department_id, dep.department_name, count(emp.department_id) as angajati 
from departments dep
RIGHT JOIN employees emp on emp.department_id=dep.department_id
GROUP BY dep.department_id, dep.department_name
having count(emp.department_id)<4  
ORDER BY dep.department_id
             b) departamentul care are numarul maxim de angajati.
 *************
select dep.department_id, dep.department_name, count(emp.department_id) as angajati
from departments dep
RIGHT JOIN employees emp ON emp.department_id = dep.department_id
GROUP BY dep.department_id, dep.department_name
HAVING COUNT(emp.department_id) = (
    SELECT MAX(angajati)
    FROM (
        SELECT COUNT(emp.department_id) AS angajati
        FROM departments dep
        RIGHT JOIN employees emp ON emp.department_id = dep.department_id
        GROUP BY dep.department_id
    )
);
 

Scrie?i un bloc anonim in care este definita o variabila in care salvez codul unui departament si salvez intr-o alta variabila codul angajatilor cu cel mai mare salariu din departamentul dat.
set serveroutput on

declare
dep number:=83;
type lista_ang is table of employees.employee_id%type;
maxsal employees.salary%type;
lista lista_ang;
begin
select employee_id bulk collect into lista
from employees right join departments using(department_id)
where nvl(salary,0) =(select nvl(max(salary),0) 
              from employees
              where department_id=dep) and department_id=dep;
if lista.count=0 then
dbms_output.put_line('dep nu exista');
elsif lista.count=1 and lista(1) is null then
  dbms_output.put_line('dep nu are angajati');
elsif lista(1) is not null then
    dbms_output.put_line('in dep '||dep||' am '||lista.count||' ang cu salariul maxim!');
end if;
end;
 

-- Aceeasi pb cu functie

create or replace function get_ang_vsl(dep in departments.department_id%type)
return lista_ang_vsl as
lista lista_ang_vsl;
begin
select employee_id bulk collect into lista
from employees right join departments using(department_id)
where nvl(salary,0) =(select nvl(max(salary),0) 
              from employees
              where department_id=dep) and department_id=dep;
return lista;
end;
 
-- Problema 1
/*Scrie?i un subprogram care returneaz? o eviden?? cu privire la num?rul de pacien?i, 
de fiecare gen, pe care i-a îngrijit fiecare angajat. Subprogramul va returna o 
singur? variabil? de tipul list? de obiecte (pentru fiecare angajat se returneaz? codul,
numele func?iei de?inute, num?rul de pacien?i de gen feminin, respectiv masculin îngriji?i).

 
create or replace type doctor is object(cod number(4), numef varchar2(20), nr_f number, nr_m number);
 
 
create or replace type lista_doctori is table of doctor;
 
 
create or replace procedure problema1(rezultat out lista_doctori) is
begin
    select doctor(id_salariat, (select nume_functie from functii where id_functie=p.id_functie),
    (select count(distinct id_pacient) 
    from pacienti join trateaza using(id_pacient)
    where gen='f' and id_salariat=p.id_salariat),
    (select count(distinct id_pacient) 
    from pacienti join trateaza using(id_pacient)
    where gen='m' and id_salariat=p.id_salariat)) bulk collect into rezultat
    from personal p;
end;
 
 
set serveroutput on
declare
ls_dr lista_doctori;
begin
problema1(ls_dr);
for i in ls_dr.first..ls_dr.last loop
dbms_output.put_line(ls_dr(i).cod||' '||ls_dr(i).numef||' '||ls_dr(i).nr_f||' '||ls_dr(i).nr_m);
end loop;
end;


-- Problema 2
/*Trigger care la ?tergerea unui angajat care este manager de specializare, 
pune în locul acestuia angajatul care a avut cei mai mul?i pacien?i 
(?i codul cel mai mic dac? sunt 2 cu aceast? proprietate). (3p).

 
--nu tratez cazul special in personal am 1 sg angajat!
 
create or replace trigger problema2 
before delete on personal
for each row 
declare
nr number;
next_sef specializare.id_manager%type;
begin
select count(*) into nr
from specializare
where id_manager=:old.id_salariat;
if nr>0 then
  select id_salariat into next_sef
  from (
  select id_salariat, count(distinct id_pacient) nr_pacienti
  from trateaza
  where id_salariat<>:old.id_salariat
  group by id_salariat
  order by nr_pacienti desc, id_salariat)
  where rownum=1;
  update specializare
  set id_manager=next_sef
  where id_manager=:old.id_salariat;
end if;
end;
 
delete from personal
where id_salariat=25;

-- Problema 3
/*Subprogram care prime?te ca parametru un cod de func?ie ?i returneaz? prin 
intermediul unui cursor lista ordonat? descresc?tor în func?ie de salariu, 
respectiv cod, a angaja?ilor care nu au tratat pacien?i ?i au func?ia transmis?. Apela?i.

 
create or replace package auxiliare is
type cursor_rezultat is ref cursor;
end;
 
create or replace function problema3(fct_data functii.id_functie%type)
return auxiliare.cursor_rezultat is
ex_fct number;
am_ang number;
rezultat auxiliare.cursor_rezultat;
begin
select count(*) into ex_fct
from functii
where id_functie=fct_data;
if ex_fct=1 then
select count(id_salariat) into am_ang
from personal
where id_functie=fct_data;
if am_ang>0 then
   open rezultat for
                select id_salariat,nume,salariu
                from personal
                where id_functie=fct_data and
                id_salariat in (select id_salariat
                                from personal
                                minus
                                select id_salariat
                                from trateaza)
                order by salariu desc, id_salariat desc;
       return rezultat;         
else
   raise_application_error(-20345,'Nu am angajati pentru fct data');
end if;
else
raise_application_error(-20346,'Functia transmisa nu exista');
end if;
end;
 
 
select problema3(1)
from dual; --in memorie cursorul ramane deschis !!!
 
 
set serveroutput on
declare
rez auxiliare.cursor_rezultat;
type ang is record(id_salariat number,nume varchar2(30), salariu number);
type linii_cursor is table of ang index by pls_integer;
linii linii_cursor;
begin
rez:=problema3(1);
fetch rez bulk collect into linii;
close rez;
for i in 1..linii.count loop
dbms_output.put_line(linii(i).id_salariat||' '||linii(i).nume||' '||linii(i).salariu);
end loop;
end;



-- Intr-un departament nu pot exista mai mult de 30 de angajati
 
select department_id, count(employee_id)
from employees right join departments using(department_id)
group by department_id;
 
create or replace package p_aux is
type info is record(nr number, info number(1));
type deps_info is table of info 
index by pls_integer;
deps deps_info;
maxang number:=30;
end;
 
 
create or replace trigger b_t_action 
before insert or update of department_id on employees
begin
p_aux.deps.delete;
for d in (select department_id, count(employee_id) nr
            from employees right join departments using(department_id)
            group by department_id) loop
     p_aux.deps(d.department_id).nr:=d.nr;
     p_aux.deps(d.department_id).info:=0;     
end loop;
end;
 
 
create or replace trigger b_r_action 
before insert or update of department_id on employees
for each row
begin
if inserting and  p_aux.deps(:new.department_id).nr>=p_aux.maxang then
   raise_application_error(-20234,'Prea multi angajati la insert!');
end if;
  p_aux.deps(:new.department_id).nr:=p_aux.deps(:new.department_id).nr+1;
  p_aux.deps(:new.department_id).info:=1;
if updating then
   p_aux.deps(:old.department_id).nr:=p_aux.deps(:old.department_id).nr-1;
end if;
end;
 
 
create or replace trigger a_t_action 
after update of department_id on employees
declare
poz number;
begin
poz:=p_aux.deps.first;
for i in 1..p_aux.deps.count loop
  if p_aux.deps(poz).info=1 and p_aux.deps(poz).nr>p_aux.maxang then
   raise_application_error(-20234,'Prea multi angajati la update!');
  end if;
  poz:=p_aux.deps.next(poz);
end loop;
end;

*/


