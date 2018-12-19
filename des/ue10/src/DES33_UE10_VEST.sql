-- 2.1 mit Johann Hoffmann
GRANT SELECT ON departments TO s1710307108;
COMMIT;

SELECT *
FROM s1710307108.departments;

-- 2.2
SELECT table_name
FROM user_tables;

SELECT table_name, owner, privilege
FROM all_tables
       INNER JOIN all_tab_privs USING (table_name)
WHERE owner <> 'S1710307099'
  AND privilege = 'SELECT';

-- 3.1
CREATE TABLE my_employee
AS
SELECT *
FROM employees;

-- 3.2
-- pre update
SELECT *
FROM my_employee
WHERE job_id LIKE '%MGR'
   OR job_id LIKE '%MAN';

UPDATE my_employee
SET salary = salary * 1.05
WHERE job_id LIKE '%MGR'
   OR job_id LIKE '%MAN';

-- post update
SELECT *
FROM my_employee
WHERE job_id LIKE '%MGR'
   OR job_id LIKE '%MAN';

COMMIT;

-- 3.3
UPDATE my_employee
SET salary = 6100
WHERE salary <= 5000;

-- should yield empty table
SELECT *
FROM my_employee
WHERE salary <= 5000;

SAVEPOINT after_salary_raise;

-- 3.4
DELETE
FROM my_employee;

SELECT *
FROM my_employee;

-- 3.5
ROLLBACK TO after_salary_raise;

SELECT *
FROM my_employee;

COMMIT; -- updates only

DROP TABLE my_employee;

-- 4.1
-- Session 1
UPDATE emp
SET sal = 6000
WHERE empno = 7839;

-- Session 2 -- muss commit von S1 abwarten!
UPDATE emp
SET sal = 7000
WHERE empno = 7839;

-- Session 1
SELECT *
FROM emp
WHERE empno = 7839; -- liest sal = 6000 (Wert von S1)

COMMIT; -- erst jetzt darf S2 schreiben

-- 4.2
-- Session 1
SELECT *
FROM emp
WHERE sal > 2800;

SELECT *
FROM dept
WHERE dname = 'RESEARCH';

-- Session 2
SELECT *
FROM dept
WHERE dname = 'RESEARCH';

SELECT *
FROM emp
WHERE sal > 2800;

-- Session 1
UPDATE emp
SET sal = sal * 1.05
WHERE sal > 2800;

-- Session 2
UPDATE dept
SET dname = 'MARKETING'
WHERE dname = 'RESEARCH';

UPDATE emp
SET sal = sal * 1.05
WHERE sal > 2800;

-- Session 1
UPDATE dept
SET dname = 'MARKETING'
WHERE dname = 'RESEARCH';

COMMIT;