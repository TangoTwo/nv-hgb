-- 1.1
CREATE TABLE customer_detail AS
SELECT customer_id,
       first_name,
       last_name,
       email,
       phone,
       district,
       postal_code,
       city,
       country
FROM customer
       INNER JOIN address USING (address_id)
       INNER JOIN city USING (city_id)
       INNER JOIN country USING (country_id);

-- 1.2
SELECT *
FROM customer_detail
WHERE last_name LIKE 'MAR%';

-- 1.3
CREATE INDEX customer_detail_lname ON customer_detail (last_name);

SELECT *
FROM customer_detail
WHERE last_name LIKE 'MAR%';

--1.4

SELECT *
FROM customer_detail
WHERE SUBSTR(last_name, 0, 3) = 'MAR';

--1.5
CREATE INDEX customer_detail_lname_substr ON customer_detail (SUBSTR(last_name, 0, 3));

SELECT *
FROM customer_detail
WHERE SUBSTR(last_name, 0, 3) = 'MAR';

-- 2.1
CREATE INDEX customer_detail_country ON customer_detail (country);

SELECT *
FROM customer_detail
WHERE country = 'India'
  AND last_name LIKE 'MAR%';

-- 2.2
SELECT /*+ INDEX (customer_detail customer_detail_country)*/ *
FROM customer_detail
WHERE country = 'India'
  AND last_name LIKE 'MAR%';

-- 3.1
-- unoptimized
SELECT DISTINCT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN ('Comedy', 'Family', 'Children')
  AND length < 100
UNION
SELECT DISTINCT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN ('Classics', 'Animation');

-- optimized
SELECT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE (name IN ('Comedy', 'Family', 'Children') AND length < 100)
   OR name IN ('Classics', 'Animation');

-- testing for equal resulting relation
SELECT DISTINCT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN ('Comedy', 'Family', 'Children')
  AND length < 100
UNION
SELECT DISTINCT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN ('Classics', 'Animation')
MINUS
SELECT customer_id, first_name, last_name
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE (name IN ('Comedy', 'Family', 'Children') AND length < 100)
   OR name IN ('Classics', 'Animation');

-- 3.2
-- unoptimized
SELECT c.customer_id,
       first_name,
       last_name,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '13') AS umsatz13,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '14') AS umsatz14,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '15') AS umsatz15,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id)        AS umsatzgesamt
FROM customer c
WHERE customer_id = 16;

-- optimized
SELECT *
FROM (
       SELECT COALESCE(to_char(payment_date, 'yy'), 'total') AS year,
              customer_id,
              first_name,
              last_name,
              SUM(amount)                                    AS umsatz
       FROM customer
              INNER JOIN payment USING (customer_id)
       GROUP BY GROUPING SETS (
         (to_char(payment_date, 'yy'), customer_id, first_name, last_name),
         (customer_id, first_name, last_name)
         )
     )
       PIVOT (
              AVG(umsatz)
              FOR year
              IN ('13' AS umsatz13,
                '14' AS umsatz14,
                '15' AS umsatz15,
                'total' AS umsatzgesamt)
    );

-- testing for equal resulting relation
SELECT c.customer_id,
       first_name,
       last_name,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '13') AS umsatz13,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '14') AS umsatz14,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id
          AND to_char(payment_date, 'yy') = '15') AS umsatz15,
       (SELECT SUM(amount)
        FROM payment
        WHERE c.customer_id = customer_id)        AS umsatzgesamt
FROM customer c
MINUS
SELECT *
FROM (
       SELECT COALESCE(to_char(payment_date, 'yy'), 'total') AS year,
              customer_id,
              first_name,
              last_name,
              SUM(amount)                                    AS umsatz
       FROM customer
              INNER JOIN payment USING (customer_id)
       GROUP BY GROUPING SETS (
         (to_char(payment_date, 'yy'), customer_id, first_name, last_name),
         (customer_id, first_name, last_name)
         )
     )
       PIVOT (
              AVG(umsatz)
              FOR year
              IN ('13' AS umsatz13,
                '14' AS umsatz14,
                '15' AS umsatz15,
                'total' AS umsatzgesamt)
    );

-- 3.3
-- unoptimized
CREATE OR REPLACE FUNCTION num_longer_films_in_cat(filmid IN NUMBER)
  RETURN NUMBER
AS
  categoryid NUMBER;
  len NUMBER;
  numfilms NUMBER := 0;
BEGIN
  SELECT category_id, length INTO categoryid, len
  FROM film
         INNER JOIN film_category USING (film_id)
  WHERE film_id = filmid;

  FOR film IN (SELECT *
               FROM film
                      INNER JOIN film_category USING (film_id)
               WHERE category_id = categoryid
                 AND length > len)
    LOOP
      numfilms := numfilms + 1;
    END LOOP;

  RETURN numfilms;
END;
/

SELECT film_id, title, num_longer_films_in_cat(film_id) AS longerfilmsincategory
FROM film;

-- optimized
SELECT film_id,
       title,
       RANK() OVER (
         PARTITION BY category_id
         ORDER BY length DESC) - 1 AS longerfilmsincategory
FROM film
       INNER JOIN film_category USING (film_id)
ORDER BY film_id;

-- testing for equal resulting relation
SELECT film_id, title, num_longer_films_in_cat(film_id) AS longerfilmsincategory
FROM film
MINUS
SELECT film_id,
       title,
       RANK() OVER (
         PARTITION BY category_id
         ORDER BY length DESC) - 1 AS longerfilmsincategory
FROM film
       INNER JOIN film_category USING (film_id)
ORDER BY film_id;

DROP FUNCTION num_longer_films_in_cat;
DROP INDEX customer_detail_lname;
DROP INDEX customer_detail_lname_substr;
DROP INDEX customer_detail_country;
DROP TABLE customer_detail;