

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

