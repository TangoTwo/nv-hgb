-- presets
CREATE TABLE new_emps AS
SELECT employee_id, last_name, salary, department_id
FROM employees;

CREATE TABLE new_locs AS
SELECT l.location_id, l.city, l.country_id
FROM locations l;

CREATE TABLE new_depts AS
SELECT d.department_id,
       d.department_name,
       AVG(e.salary) AS dept_sal,
       d.location_id
FROM employees e
       INNER JOIN departments d
                  ON (e.department_id = d.department_id)
GROUP BY d.department_id, d.department_name, d.location_id;

CREATE TABLE new_countries AS
SELECT c.country_id, c.country_name, COUNT(e.employee_id) AS c_emps
FROM countries c
       INNER JOIN locations l ON (l.country_id = c.country_id)
       INNER JOIN departments d ON (d.location_id = l.location_id)
       INNER JOIN employees e ON (e.department_id = d.department_id)
GROUP BY c.country_id, c.country_name;

-- 1.1
CREATE OR REPLACE PROCEDURE check_salary(jid IN jobs.job_id%TYPE, salary IN jobs.min_salary%TYPE) IS
  min_sal jobs.min_salary%TYPE;
  max_sal jobs.max_salary%TYPE;
  salary_too_high EXCEPTION;
BEGIN
  -- get salaries from job
  SELECT min_salary,
         max_salary
         INTO min_sal, max_sal
  FROM jobs
  WHERE job_id = jid;

  IF (salary > max_sal) THEN -- if max salary < salary -> raise exception
    RAISE salary_too_high;
  ELSIF (salary < min_sal) THEN -- if min salary > salary -> change min salary
    UPDATE jobs
    SET min_salary = salary
    WHERE job_id = jid;
  END IF;

  EXCEPTION
  WHEN salary_too_high
  THEN
    RAISE_APPLICATION_ERROR(-20202, 'Invalid salary ' || salary || '. Salary too high for job ' || jid || '.');
END;

BEGIN
  check_salary('AD_PRES', 50000);
END;
/

-- 1.2
CREATE OR REPLACE TRIGGER check_salary_trg
  BEFORE INSERT OR UPDATE
  ON employees
  FOR EACH ROW
BEGIN
  CHECK_SALARY(:new.job_id, :new.salary);
END;

-- 1.3
UPDATE employees
SET salary = 10000
WHERE employee_id = 100;

SELECT *
FROM employees
WHERE employee_id = 100;

UPDATE employees
SET salary = 50000
WHERE employee_id = 100;

-- reset to original value in case future
-- exercises depend on that value
ROLLBACK;

-- 1.5
ALTER TABLE employees
  ADD (date_modified DATE, user_modified VARCHAR2(255));

CREATE OR REPLACE TRIGGER log_employees
  BEFORE INSERT OR UPDATE
  ON employees
  FOR EACH ROW
BEGIN
  :new.user_modified := USER;
  :new.date_modified := SYSDATE;
END;

-- TEST: default use case
UPDATE employees
SET salary = 19000
WHERE employee_id = 101;

SELECT employee_id, salary, date_modified, user_modified
FROM employees
WHERE employee_id = 101;

-- TEST: invalid employee
UPDATE employees
SET salary = 19000
WHERE employee_id = 10;

-- TEST: salary > max_salary
UPDATE employees
SET salary = 50000
WHERE employee_id = 100;

ROLLBACK;

-- 2.1
CREATE OR REPLACE VIEW emp_details AS
SELECT employee_id,
       last_name,
       salary,
       department_id,
       department_name,
       dept_sal,
       location_id,
       city,
       country_name,
       c_emps
FROM new_emps
       INNER JOIN new_depts USING (department_id)
       INNER JOIN new_locs USING (location_id)
       INNER JOIN new_countries USING (country_id);

SELECT *
FROM user_updatable_columns
WHERE table_name = 'EMP_DETAILS';

-- 2.2
CREATE OR REPLACE TRIGGER dml_emp_details
  INSTEAD OF INSERT OR UPDATE OR DELETE
  ON emp_details
  FOR EACH ROW
BEGIN
  IF INSERTING THEN -- (b)
    -- insert employee
    INSERT INTO new_emps
    VALUES (:new.employee_id, :new.last_name, :new.salary, :new.department_id);

    -- raise dept_sal
    UPDATE new_depts
    SET dept_sal = (SELECT AVG(salary) FROM new_emps WHERE department_id = :new.department_id)
    WHERE department_id = :new.department_id;

    -- increase country "population"
    UPDATE new_countries
    SET c_emps = c_emps + 1
    WHERE country_name = :new.country_name;

  ELSIF UPDATING ('salary') THEN -- (c)
    -- update salary
    UPDATE new_emps
    SET salary = :new.salary
    WHERE employee_id = :new.employee_id;

    -- also update average department salary
    UPDATE new_depts
    SET dept_sal = (SELECT AVG(salary) FROM new_emps WHERE department_id = :new.department_id)
    WHERE department_id = :new.department_id;

  ELSIF UPDATING ('department_id') THEN -- (d)
    -- update employee dept_id
    UPDATE new_emps
    SET department_id = :new.department_id
    WHERE employee_id = :new.employee_id;

    -- update average salary for both involved depts
    UPDATE new_depts
    SET dept_sal = (SELECT AVG(salary) FROM new_emps WHERE department_id = :old.department_id)
    WHERE department_id = :old.department_id;
    UPDATE new_depts
    SET dept_sal = (SELECT AVG(salary) FROM new_emps WHERE department_id = :new.department_id)
    WHERE department_id = :new.department_id;

    -- update country "population"
    UPDATE new_countries
    SET c_emps = (SELECT COUNT(*) FROM new_emps WHERE department_id = :old.department_id)
    WHERE country_name = :new.country_name;


  ELSIF DELETING THEN -- (a)
    -- delete from employees
    DELETE
    FROM new_emps
    WHERE employee_id = :old.employee_id;

    -- lower dept_sal
    UPDATE new_depts
    SET dept_sal = (SELECT AVG(salary) FROM new_emps WHERE department_id = :old.department_id)
    WHERE department_id = :old.department_id;

    -- decrease country "population"
    UPDATE new_countries
    SET c_emps = c_emps - 1
    WHERE country_name = :old.country_name;
  END IF;
END;

SELECT employee_id, last_name, salary, department_name, ROUND(dept_sal, 2), c_emps
FROM emp_details
ORDER BY employee_id;

INSERT INTO emp_details (employee_id,
                         last_name,
                         salary,
                         department_id,
                         country_name)
VALUES (1, 'HEHE', 17000, 110, 'Canada');

UPDATE emp_details
SET salary = 16000
WHERE employee_id = 1;

UPDATE emp_details
SET department_id = 90
WHERE employee_id = 1;

DELETE
FROM emp_details
WHERE employee_id = 1;

ROLLBACK;

-- 3.1
CREATE TABLE user_logging
(
  session_id NUMBER,
  login_time DATE          NOT NULL,
  db_user    VARCHAR2(255) NOT NULL,
  os_user    VARCHAR2(255) NOT NULL,
  ip         VARCHAR2(15)  NOT NULL,
  host_name  VARCHAR2(255) NOT NULL
);

CREATE OR REPLACE TRIGGER user_logging_trg
  AFTER LOGON ON SCHEMA
BEGIN
  INSERT INTO user_logging
  VALUES (SYS_CONTEXT('USERENV', 'SESSIONID'),
          SYSDATE,
          USER,
          SYS_CONTEXT('USERENV', 'OS_USER'),
          SYS_CONTEXT('USERENV', 'IP_ADDRESS'),
          SYS_CONTEXT('USERENV', 'HOST'));

  -- A failing trigger can prevent user login
  -- so I added this exception handler, hoping that
  -- this is all it takes to bypass this behaviur.
  EXCEPTION
  WHEN OTHERS
  THEN
    dbms_output.PUT_LINE('Error in trigger!');
END;

SELECT *
FROM user_logging;

DROP PROCEDURE check_salary;
DROP TRIGGER check_salary_trg;
DROP TRIGGER log_employees;
DROP TRIGGER dml_emp_details;
DROP VIEW emp_details;
DROP TABLE new_emps;
DROP TABLE new_depts;
DROP TABLE new_locs;
DROP TABLE new_countries;
DROP TRIGGER user_logging_trg;
DROP TABLE user_logging;
ALTER TABLE employees
  DROP (date_modified, user_modified);