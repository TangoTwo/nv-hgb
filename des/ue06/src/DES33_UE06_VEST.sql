-- 1.1
DROP TABLE top_salaries;
CREATE TABLE top_salaries (
  salary NUMBER(8, 2)
);

-- 1.2
DELETE FROM top_salaries;

DECLARE
  -- employee
  num NUMBER(3) := &p_num;
  -- salary
  sal employees.salary%TYPE;
  -- employee cursor, highest salary first
  CURSOR emp_cursor IS
    SELECT DISTINCT salary
    FROM employees
    ORDER BY salary DESC;
BEGIN
  OPEN emp_cursor;
  FETCH emp_cursor INTO sal;
  -- for as long as more rows should be read
  -- and while there are still more rows to process
  WHILE emp_cursor%ROWCOUNT <= num AND emp_cursor%FOUND LOOP
    -- add dat salary to the top_salaries table
    INSERT INTO top_salaries (salary)
    VALUES (sal);
    -- fetch next tuple
    FETCH emp_cursor INTO sal;
  END LOOP;
  CLOSE emp_cursor;
END;
/

SELECT * FROM top_salaries;

-- 1.4
ALTER TABLE top_salaries
    ADD (emp_cnt NUMBER DEFAULT 1 NOT NULL)
    ADD CONSTRAINT emp_cnt_gt_0 CHECK(emp_cnt > 0)
    ADD CONSTRAINT top_salaries_pk PRIMARY KEY(salary);

-- 1.5
DELETE FROM top_salaries;

DECLARE
  -- employee
  num NUMBER(3) := &p_num;
  top_sal top_salaries%ROWTYPE;
  -- employee cursor, highest salary first
  CURSOR emp_cursor IS
    SELECT salary, COUNT(*)
    FROM employees
    GROUP BY salary
    ORDER BY salary DESC;
BEGIN
  OPEN emp_cursor;
  FETCH emp_cursor INTO top_sal;
  -- for as long as more rows should be read
  -- and while there are still more rows to process
  WHILE emp_cursor%ROWCOUNT <= num AND emp_cursor%FOUND LOOP
    -- add dat salary to the top_salaries table
    INSERT INTO top_salaries (salary, emp_cnt)
    VALUES (top_sal.salary, top_sal.emp_cnt);
    -- fetch next tuple
    FETCH emp_cursor INTO top_sal;
  END LOOP;
  CLOSE emp_cursor;
END;
/

SELECT * FROM top_salaries;

-- 2.1
DELETE FROM top_salaries;
ALTER TABLE top_salaries
  ADD (created_by    VARCHAR2(50)  DEFAULT ''      NOT NULL)
  ADD (date_created  DATE           DEFAULT SYSDATE NOT NULL)
  ADD (modified_by   VARCHAR2(50)  DEFAULT ''      NOT NULL)
  ADD (date_modified DATE           DEFAULT SYSDATE NOT NULL);

DECLARE
  -- employee
  num NUMBER(3) := &p_num;
  -- salary
  sal employees.salary%TYPE;
  cnt top_salaries.emp_cnt%TYPE;
  -- employee cursor, highest salary first
  CURSOR emp_cursor IS
    SELECT salary, COUNT(*)
    FROM employees
    GROUP BY salary
    ORDER BY salary DESC;
BEGIN
  OPEN emp_cursor;
  FETCH emp_cursor INTO sal, cnt;
  -- for as long as more rows should be read
  -- and while there are still more rows to process
  WHILE emp_cursor%ROWCOUNT <= num AND emp_cursor%FOUND LOOP
    -- add dat salary to the top_salaries table
    INSERT INTO top_salaries (salary, emp_cnt, date_created, created_by, date_modified, modified_by)
    VALUES (sal, cnt, SYSDATE, USER, SYSDATE, USER);
    -- fetch next tuple
    FETCH emp_cursor INTO sal, cnt;
  END LOOP;
  CLOSE emp_cursor;
END;
/

SELECT * FROM top_salaries;

-- 2.2
ALTER SESSION SET NLS_DATE_FORMAT = 'dd.mm.yyyy hh24:mi:ss';

CREATE OR REPLACE PROCEDURE InsertTopSalaries (pSalary IN NUMBER, pEmp_cnt IN NUMBER)
  IS
  BEGIN
    INSERT INTO top_salaries (salary, emp_cnt, date_created, created_by, date_modified, modified_by)
    VALUES (psalary, pEmp_cnt, SYSDATE, USER, SYSDATE, USER);
  END;
  /

-- 2.3
DELETE FROM top_salaries;
DECLARE
  -- employee
  num NUMBER(3) := &p_num;
  -- salary
  sal employees.salary%TYPE;
  cnt top_salaries.emp_cnt%TYPE;
  -- employee cursor, highest salary first
  CURSOR emp_cursor IS
    SELECT salary, COUNT(*)
    FROM employees
    GROUP BY salary
    ORDER BY salary DESC;
BEGIN
  OPEN emp_cursor;
  FETCH emp_cursor INTO sal, cnt;
  -- for as long as more rows should be read
  -- and while there are still more rows to process
  WHILE emp_cursor%ROWCOUNT <= num AND emp_cursor%FOUND LOOP
    -- add dat salary to the top_salaries table
    InsertTopSalaries(sal, cnt);
    -- fetch next tuple
    FETCH emp_cursor INTO sal, cnt;
  END LOOP;
  CLOSE emp_cursor;
END;
/

SELECT * FROM top_salaries;

-- 3.
CREATE TABLE my_payment AS
  SELECT *
  FROM payment
  WHERE rental_id IS NOT NULL;
ALTER TABLE my_payment ADD PRIMARY KEY (payment_id);
ALTER TABLE my_payment ADD penalty NUMBER;

-- UPDATE in loop
DECLARE
  starttime NUMBER;
  total NUMBER;
  maxRent NUMBER := 0;
  actualRent NUMBER := 0;
BEGIN
  starttime := DBMS_UTILITY.GET_TIME();
  FOR mp IN (SELECT amount, rental_id, payment_id, payment_date FROM my_payment) LOOP
    SELECT MAX(rental_duration) INTO maxRent
    FROM film
           INNER JOIN inventory USING (film_id)
           INNER JOIN rental USING (inventory_id)
    WHERE rental_id = mp.rental_id;
    SELECT MAX(CEIL(return_date - rental_date)) INTO actualRent
    FROM rental
    WHERE rental_id = mp.rental_id;

    IF actualRent > maxRent THEN
      UPDATE my_payment
      SET penalty = amount * 1.15
      WHERE mp.payment_id = payment_id;
    END IF;

  END LOOP;
  total := DBMS_UTILITY.GET_TIME() - starttime;
  DBMS_OUTPUT.PUT_LINE('PL/SQL LOOP: ' || total / 100 || ' seconds');
END;
/

-- optimized version
DECLARE
  starttime NUMBER;
  total NUMBER;
BEGIN
  starttime := DBMS_UTILITY.GET_TIME();

  UPDATE my_payment mp
  SET penalty = amount * 1.15
  WHERE EXISTS (SELECT r.rental_id
          FROM rental r
                 INNER JOIN inventory i ON r.inventory_id = i.inventory_id
                 INNER JOIN film f ON i.film_id = f.film_id
          WHERE r.rental_id = mp.rental_id AND
            CEIL(return_date - rental_date) > f.rental_duration);

  total := DBMS_UTILITY.GET_TIME() - starttime;
  DBMS_OUTPUT.PUT_LINE('PL/SQL WITHOUT LOOP: ' || total / 100 || ' seconds');
END;
/

DROP TABLE my_payment;