-- 1.1a
SELECT employee_id,
       last_name,
       hire_date,
       salary
FROM employees
    -- basically WHERE manager_id = 102;
START WITH manager_id = 102
CONNECT BY PRIOR employee_id = manager_id
       AND level = 1;

-- 1.1b
SELECT employee_id,
       last_name,
       hire_date,
       salary
FROM employees
START WITH employee_id = 102
CONNECT BY PRIOR employee_id = manager_id;

-- 1.2
SELECT employee_id,
       last_name,
       hire_date,
       salary
FROM employees
WHERE level = 3
START WITH employee_id = 102
CONNECT BY PRIOR employee_id = manager_id;

-- 1.3
SELECT employee_id,
       manager_id,
       LPAD(last_name, LENGTH(last_name) + level, '-') AS hierarchy
FROM employees
START WITH manager_id IS NULL
CONNECT BY PRIOR employee_id = manager_id;

-- 1.4
WITH superiors AS (SELECT CONNECT_BY_ROOT first_name AS first_name,
                          CONNECT_BY_ROOT last_name AS last_name
                   FROM employees
                   WHERE level > 1
                   CONNECT BY PRIOR employee_id = manager_id)
SELECT first_name,
       last_name,
       COUNT(*)
FROM superiors
GROUP BY first_name, last_name;

-- 2
CREATE OR REPLACE VIEW partners AS
  SELECT a1.actor_id AS actor_id,
         a2.actor_id AS partner_id,
         a1.film_id AS film_id
  FROM film_actor a1
         INNER JOIN film_actor a2 ON a1.actor_id != a2.actor_id AND
                                     a1.film_id = a2.film_id
  WHERE a1.film_id <= 13;

-- Nick Wahlberg is has actor_id 2!
SELECT DISTINCT partner_id,
                last_name,
                first_name
FROM partners p
  INNER JOIN actor a ON p.partner_id = a.actor_id
WHERE partner_id NOT IN
      (SELECT partner_id
       FROM partners
       WHERE actor_id = 2)
START WITH p.actor_id = 2
CONNECT BY NOCYCLE PRIOR partner_id = p.actor_id
       AND partner_id != 2
       AND level = 2;

-- 3.1
SELECT *
FROM
     (SELECT staff_id,
             name AS category
      FROM rental
             INNER JOIN inventory USING (inventory_id)
             INNER JOIN film USING (film_id)
             INNER JOIN film_category USING (film_id)
             INNER JOIN category USING (category_id))
         PIVOT
         (COUNT(*) AS anzahl
         FOR staff_id
         IN (1 AS verk1, 2 AS verk2))
ORDER BY category ASC;

-- 3.2
SELECT name,
       ROUND(family, 2),
       ROUND(children, 2),
       ROUND(animation, 2)
FROM
     (SELECT lang.name,
             cat.name AS category,
             length
      FROM category cat
             INNER JOIN film_category USING (category_id)
             INNER JOIN film USING (film_id)
             INNER JOIN language lang USING (language_id))
         PIVOT
         (AVG(length)
         FOR category
         IN ('Family' AS family, 'Children' AS children, 'Animation' AS animation));

-- 3.3
SELECT *
FROM
     (SELECT title,
             l.name AS lang,
             ol.name AS original
      FROM film f
             INNER JOIN language l ON f.language_id = l.language_id
             INNER JOIN language ol ON f.original_language_id = ol.language_id
      WHERE release_year = 1983)
         UNPIVOT
         (language
         FOR kind
         IN (lang AS 'L', original AS 'OL'))
ORDER BY title ASC;

-- 4.1
SELECT title,
       rental_date,
       RANK() OVER (PARTITION BY title ORDER BY rental_date) AS rank
FROM rental
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
ORDER BY rental_date DESC
FETCH FIRST 10 ROWS WITH TIES;

-- 4.2
SELECT name,
       title,
       release_year
FROM
     (SELECT name,
             title,
             release_year,
             ROW_NUMBER() OVER (PARTITION BY category_id ORDER BY release_year) AS rn
      FROM film_category
             INNER JOIN film USING (film_id)
             INNER JOIN category USING (category_id))
WHERE rn < 4;

-- 4.3
-- I tried to partition by customer id
-- and then let a window of size 2 compare
-- 2 adjacent dates recursively but calculating
-- the date difference turned out to be quite
-- tricky so I used regular joins. :)
WITH dates AS (SELECT customer_id,
                      rental_date,
                      ROW_NUMBER() OVER (PARTITION BY customer_id ORDER BY rental_date ASC) AS rn
               FROM rental)
SELECT d1.customer_id,
       c.last_name,
       ROUND(AVG(d2.rental_date - d1.rental_date)) AS average_rental_interval
FROM dates d1
       INNER JOIN dates d2 ON d1.customer_id = d2.customer_id AND
                              d1.rn + 1 = d2.rn -- CONNECT BY (...) AND PRIOR rn + 1 = rn
       INNER JOIN customer c ON (d1.customer_id = c.customer_id)
GROUP BY d1.customer_id, c.last_name;