

## Join & agregări

1. Pentru fiecare **țară**, afișați **numărul de orașe** (locations) și **numărul de departamente** din acea țară.
2. Pentru fiecare **departament**, afișați **salariul mediu**, **salariul minim** și **numărul de angajați**; ordonați descrescător după salariul mediu.
3. Afișați **top 3 orașe** cu cei mai mulți angajați (dacă egalități, toate la poziția 3).
4. Afișați **angajații** împreună cu **numele managerului** (self-join pe `employees`). Includeți și pe cei fără manager (manager = `NULL`).

## Subinterogări & condiții

5. Afișați angajații care câștigă **peste media pe departamentul lor**.
6. Afișați **departamentele fără angajați** (există în `departments`, dar nu apar în `employees`).
7. Afișați angajații care **nu** au avut **niciodată** în `job_history` un job diferit de cel curent.
8. Afișați **angajații** care lucrează în **aceeași țară** cu **managerul lor**.

## Date & string

9. Pentru fiecare **lună** (1..12), afișați **câți angajați** au fost angajați în acea lună (indiferent de an).
10. Afișați angajații al căror **email** are exact **5 litere** înainte de `@` (dacă păstrați convenția `email` fără domeniu, adaptați cerința: selectați emailuri de **5 caractere**).
11. Afișați angajații angajați într-o **zi de luni**, cu `hire_date` și `TO_CHAR(hire_date, 'Day')`.

## Analitice

12. Pentru fiecare **job**, marcați angajații cu un flag “**Peste medie**/Nu\*\*” în funcție de media pe job (folosiți `AVG(...) OVER (PARTITION BY job_id)`).
13. Afișați, pentru fiecare **departament**, **top 2 salarii** (dacă egalități la locul 2, includeți-le pe toate) – `DENSE_RANK()`.

## Istoric & joburi

14. Afișați angajații care au avut în `job_history` un **job din altă familie** decât cel curent (de ex. `SA%` vs `IT%`), presupunând prefixe semnificative în `job_id`.
15. Afișați angajații care au revenit la **același job** după ce au avut între timp alt job (pattern „A → B → A”).

## Proiecte & alocări (tabelele `projects`, `works_on`)

16. Pentru fiecare **proiect**, afișați **numărul de angajați** alocați și **numărul total de zile** lucrate (suma `end_date - start_date`).
17. Afișați **angajații** care au lucrat pe **cel puțin două proiecte diferite**.
18. Afișați **managerii de proiect** (`projects.project_manager`) care **nu** sunt alocați **ca resursă** în `works_on` pe propriul lor proiect.
19. Afișați **proiectele întârziate** (unde `delivery_date` > `deadline`) și **numărul de zile** de întârziere.
20. Afișați **angajații** care au lucrat pe **proiecte** în **cel puțin două luni calendaristice diferite** (pe baza intervalelor din `works_on`).

## Bonus – gândire de interviu

21. Afișați **top 3 țări** cu cele mai multe **angajări** (număr de angajați), apoi – pentru fiecare astfel de țară – afișați **departamentul** cu **cel mai mare salariu mediu**.
22. Pentru fiecare **job**, afișați **distribuția pe intervale** în `JOB_GRADES` (în ce **grade** cade salariul fiecărui angajat); numărați câți angajați are fiecare job în fiecare grade.

---

Mai multe exercitii

## Join & agregări

1. Pentru fiecare **țară**, afișați **numărul de orașe** (locations) și **numărul de departamente** din acea țară.
2. Pentru fiecare **oraș**, afișați **numărul de angajați** și **media salariilor**, ordonat descrescător după numărul de angajați (cu ties).
3. Afișați **top 5 job-uri** cu cel mai mare **salariu mediu** (cu ties).
4. Pentru fiecare **departament**, afișați **salariul minim, maxim și mediu** și numărul de angajați; excludeți departamentele fără angajați.

## Subinterogări & EXISTS

5. Afișați angajații care câștigă **peste media pe țara lor**.
6. Afișați **departamentele fără angajați** (anti-join/NOT EXISTS).
7. Afișați angajații care **nu** au avut niciodată în `job_history` un **job diferit** de cel curent.
8. Afișați angajații care lucrează în **aceeași țară** cu **managerul** lor.

## Date & șiruri

9. Pentru fiecare **lună (1..12)**, afișați câți angajați au fost angajați în acea lună (indiferent de an) și procentul din total.
10. Afișați angajații al căror **email** are exact **6 caractere** înainte de `@` (sau exact 6 caractere dacă e fără domeniu).
11. Afișați angajații angajați într-o **zi de vineri**, cu `TO_CHAR(hire_date, 'FMDAY', 'NLS_DATE_LANGUAGE=ENGLISH')`.
12. Afișați angajații care au aceeași **zi a lunii** la `hire_date` ca și **managerul** lor.

## Ferestre analitice (window functions)

13. Pentru fiecare **job**, marcați angajații cu “**Peste medie**/Nu” față de media pe job (AVG OVER PARTITION BY).
14. Pentru fiecare **departament**, afișați **top 2 salarii** (includeți toate egalitățile la poziția 2) — `DENSE_RANK()`.
15. Afișați **top 3 orașe** după numărul de angajați, folosind `FETCH FIRST 3 ROWS WITH TIES` după agregare.
16. Pentru fiecare **job**, afișați **percentila 90** a salariului (`PERCENTILE_CONT(0.9) WITHIN GROUP (ORDER BY salary)`), apoi angajații peste această valoare.

## Istoric & carieră

17. Afișați angajații care au ocupat **de mai multe ori același job** (doar `JOB_HISTORY`).
18. Afișați angajații care au avut în `JOB_HISTORY` un **job din altă familie** decât cel curent (prefix de 2 litere).
19. Afișați angajații care au revenit la **același job** după un alt job (tipar **A → B → A**).
20. Afișați angajații al căror salariu **nu se încadrează** în niciun interval din `JOB_GRADES`.

## Proiecte (`PROJECTS`, `WORKS_ON`)

21. Pentru fiecare **proiect**, afișați **numărul de angajați** și **totalul de zile** lucrate (suma `end_date - start_date`), ignorând intervalele invalide (`end_date < start_date`).
22. Afișați angajații care au lucrat pe **cel puțin două proiecte diferite**.
23. Afișați **proiectele întârziate** (`delivery_date > deadline`) și **numărul de zile** de întârziere (TRUNC la zile întregi).
24. Afișați **managerii de proiect** care **nu** sunt alocați ca resursă pe propriul proiect (anti-join pe `WORKS_ON`).
25. Afișați angajații care au lucrat pe proiecte în **cel puțin 3 luni calendaristice diferite** (expandați lunile sau folosiți min/max de lună).
26. Afișați, pentru fiecare **proiect**, **primii 2** angajați cu cele mai multe **zile** lucrate la acel proiect (includeți ties) — `DENSE_RANK()` peste `SUM(end_date - start_date)`.

## Bonus (gândire/interviu)

27. Afișați **top 3 țări** după numărul de angajați și, pentru fiecare, **departamentul(ele)** cu **salariul mediu maxim** (folosiți `WITH TIES` sau `DENSE_RANK`).
28. Afișați angajații care au **subordonați** ce câștigă **mai mult** decât ei (self-join pe manager).
29. Afișați, pentru fiecare **regiune**, **top 1 oraș** după numărul de angajați (cu ties) — vedeți lanțul `employees → departments → locations → countries → regions`.
30. Afișați **media vechimii** (în ani) pe departament, calculată din `MONTHS_BETWEEN(SYSDATE, hire_date)`.

