-- 1.1
SELECT first_name || ' ' || last_name AS "Name"
FROM actor
ORDER BY last_name ASC;

-- 1.2
SELECT
       title,
       length
FROM film
WHERE length < 50;

-- 1.3
SELECT INITCAP(title) AS "Name"
FROM film
WHERE title LIKE '___A%';

-- 1.4
SELECT title
FROM film
WHERE original_language_id IS NOT NULL;

-- 1.5
SELECT COUNT(*) AS "Nr. of rented films"
FROM rental
WHERE rental_date < TO_DATE('31.12.2015', 'DD.MM.YYYY') AND
        rental_date > TO_DATE('01.01.2015', 'DD.MM.YYYY');

-- 1.6
SELECT inventory_id
FROM inventory
MINUS
SELECT inventory_id
FROM rental;

-- 1.7
SELECT
       first_name,
       last_name
FROM customer
       INNER JOIN address USING (address_id)
       INNER JOIN city USING (city_id)
WHERE city IN ('Newcastle', 'Linz', 'London');

-- 1.8
SELECT
       to_char(rental_date, 'Day, DD.Mon.YYYY') AS rental_date,
       amount
FROM payment p
       INNER JOIN rental r USING (rental_id)
WHERE p.customer_id = 420;

-- 2.1
SELECT
       ROUND(AVG(rental_rate), 2),
       category_id
FROM film
       INNER JOIN film_category USING (film_id)
GROUP BY category_id;

-- 2.2
SELECT title
FROM film
WHERE length > (SELECT length
                FROM film
                WHERE film_id = 50) AND
        replacement_cost > (SELECT replacement_cost
                            FROM film
                            WHERE film_id = 101);

-- 2.3
SELECT title
FROM film
       INNER JOIN film_category USING (film_id)
WHERE length < 60 AND
        category_id IN (SELECT category_id
                        FROM film
                               INNER JOIN film_category USING (film_id)
                        WHERE film_id IN (10, 20, 30));

-- 2.4
SELECT
       first_name,
       last_name, Count(*) AS "Nr. of films"
FROM actor
       INNER JOIN film_actor USING (actor_id)
WHERE actor_id IN (SELECT actor_id
                   FROM film_actor
                   GROUP BY actor_id
                   HAVING COUNT(*) > 35)
GROUP BY actor_id, first_name, last_name;

-- 2.5
SELECT
       title,
       length
FROM film
WHERE length > (SELECT AVG(length)
                FROM film);

-- 2.6
SELECT UNIQUE name
FROM film_category
       INNER JOIN category USING (category_id)
WHERE category_id IN (SELECT category_id
                      FROM film_category
                      GROUP BY category_id
                      HAVING COUNT(film_id) < 60);

-- 2.7
SELECT
       title,
       length,
       release_year
FROM film f1
WHERE length >= ALL (SELECT length
                     FROM film f2
                     WHERE f2.release_year = f1.release_year);

-- 2.8
SELECT
       film_id,
       to_date(rental_date, 'YYYY-MM-DD') AS rental_date
FROM (SELECT *
      FROM rental
             INNER JOIN inventory USING (inventory_id)
             INNER JOIN film USING (film_id)
      ORDER BY rental_date DESC)
FETCH FIRST 9 ROWS ONLY;

-- 3.1
CREATE TABLE new_film
  AS (SELECT *
      FROM film
      WHERE release_year >= (SELECT MAX(release_year) FROM film));

-- 3.2
INSERT INTO new_film
    (film_id, title, language_id, rental_duration, rental_rate, replacement_cost, release_year, last_update)
VALUES
     (1001, 'Jason Bourne', 1, 5, 1.75, 16.99, 2016, (SELECT sysdate FROM dual));

-- 3.3
UPDATE new_film
SET rental_rate = rental_rate * 1.15
WHERE rental_rate < 2;

-- 3.4
CREATE OR REPLACE VIEW cheap_film
  AS SELECT
         title,
         description,
         rental_rate,
         length
  FROM new_film
  WHERE rental_rate <= 2
  WITH CHECK OPTION;

-- 3.5: COMMIT and ROLLBACK does not do anything since
--  data-dictionary manipulations are followed by an automatic commit

-- 3.6
UPDATE cheap_film
SET rental_rate = rental_rate * 1.15
WHERE rental_rate < 2;
-- Does not work since some films' rental_rate would
-- exceed the upper boundary (2)

-- 3.7
DELETE
FROM new_film
WHERE rental_rate > 1.79;

-- 3.8 (1.79 * 1.15) > 2, Since those entries are deleted in 3.7, all other values (x * 1.15)

-- 3.9
DROP TABLE new_film;
DROP VIEW cheap_film;
