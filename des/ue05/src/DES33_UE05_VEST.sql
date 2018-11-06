-- 1.1
SELECT release_year, title,
       LISTAGG(SUBSTR(first_name, 0, 1) || '. ' || last_name, ', ')
           WITHIN GROUP (ORDER BY last_name) AS actors
FROM film
       INNER JOIN film_actor USING (film_id)
       INNER JOIN actor USING (actor_id)
GROUP BY film_id, title, release_year
ORDER BY release_year ASC, title ASC;

-- 1.2
SELECT first_name || ' ' || last_name AS customer,
       LISTAGG(title || ' (' || to_char(release_year) || ')', ',')
           WITHIN GROUP (ORDER BY release_year DESC) AS films
FROM rental
       INNER JOIN customer USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
WHERE (
          (SELECT sysdate
           FROM dual) - rental_date) / 365 <= 4
GROUP BY customer_id, first_name, last_name
ORDER BY last_name ASC;

-- 1.3
WITH at_least_three AS
    (SELECT customer_id,
            category_id,
            COUNT(film_id) AS films_of_category
     FROM customer c
            INNER JOIN store s ON c.store_id = s.store_id
            INNER JOIN address a ON s.address_id = a.address_id
            INNER JOIN city USING (city_id)
            INNER JOIN rental USING (customer_id)
            INNER JOIN inventory USING (inventory_id)
            INNER JOIN film USING (film_id)
            INNER JOIN film_category USING (film_id)
     WHERE city = 'Linz'
     GROUP BY customer_id, category_id
     HAVING COUNT(film_id) >= 3)
SELECT first_name || ' ' || last_name AS customer,
       LISTAGG(name, ', ')
           WITHIN GROUP (ORDER BY name ASC) AS interests
FROM at_least_three
       INNER JOIN customer USING (customer_id)
       INNER JOIN category USING (category_id)
GROUP BY customer_id, first_name, last_name
ORDER BY last_name ASC;

-- 2.1
CREATE OR REPLACE VIEW ue5_02a AS
  WITH revenues AS
  (SELECT store_id,
          category_id,
          SUM(amount) AS revenue
   FROM payment
          INNER JOIN rental USING (rental_id)
          INNER JOIN inventory USING (inventory_id)
          INNER JOIN store USING (store_id)
          INNER JOIN film_category USING (film_id)
   WHERE store_id IN (1, 2)
   GROUP BY store_id, category_id
   ORDER BY category_id ASC)
  SELECT category_id,
         store1_revenue,
         store2_revenue,
         ROUND(store1_revenue / store2_revenue, 2) AS s1_relative_to_s2
  FROM
       (SELECT *
        FROM revenues
            PIVOT
            -- it's only one row anyway
            -- so AVG should be fine
            (AVG(revenue) AS revenue
            FOR store_id
            IN (1 AS store1,
              2 AS store2)));

SELECT * FROM ue5_02a;

-- 2.2
CREATE MATERIALIZED VIEW ue05_02b
BUILD IMMEDIATE
REFRESH COMPLETE
ON DEMAND
  AS
    (SELECT *
     FROM ue5_02a);

SELECT * FROM ue05_02b;

DROP MATERIALIZED VIEW ue05_02b;
-- 2.3
CREATE MATERIALIZED VIEW ue05_02c
REFRESH COMPLETE
START WITH TRUNC(SYSDATE) + ((1/24/60) * (23 * 60 + 30))
NEXT TRUNC(SYSDATE) + 1 + ((1/24/60) * (23 * 60 + 30))
  AS
    (SELECT *
     FROM ue05_02b);

DROP MATERIALIZED VIEW ue05_02c;

-- 3.1
SELECT  column_name,
       data_type,
       data_length,
       nullable
FROM user_tab_columns
WHERE table_name = &tab_name;

-- 3.2
COMMENT ON TABLE store IS 'This is one hell of a store collection.';
SELECT table_name,
       comments
FROM user_tab_comments
WHERE table_name = 'STORE';

-- 3.3
SELECT cols.column_name,
       cols.constraint_name,
       cons.constraint_type,
       cons.search_condition,
       cons.status
FROM user_constraints cons INNER JOIN user_cons_columns cols
         ON cons.constraint_name = cols.constraint_name
WHERE cons.table_name = &tab_name;

-- 4.1
CREATE SEQUENCE probe_seq;

CREATE TABLE probe (
  id VARCHAR2(50),
  timestamp DATE NOT NULL,
  type VARCHAR2(50) NOT NULL,
  "comment" VARCHAR2(255),
  CONSTRAINT probe_pk PRIMARY KEY (id)
);

-- There must be some sort of information about
-- who is doing the insert because otherwise the
-- DB could not distinguish the teams since I assume
-- there is only one DB user for all teams (as the
-- task description suggests). Hence I decided to
-- concatenate the next sequence value with a unique
-- team descriptor.
INSERT
    INTO probe (id, timestamp, type)
  VALUES (probe_seq.nextval || '_team1', SYSDATE, 'Milch');

INSERT
    INTO probe (id, timestamp, type)
  VALUES (probe_seq.nextval || '_team2', SYSDATE, 'Eier');

SELECT * FROM probe;

DROP TABLE probe;
DROP SEQUENCE probe_seq;
