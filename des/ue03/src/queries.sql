-- 2 a
SELECT department_id, department_name, Count(*)
FROM departments
       INNER JOIN employees USING (department_id)
GROUP BY department_id, department_name
HAVING Count(*) < 3;

-- 2 b
SELECT department_id, department_name, Count(*)
FROM departments
       INNER JOIN employees USING (department_id)
GROUP BY department_id, department_name
HAVING Count(*) = (SELECT MAX(COUNT(employee_id))
                   FROM departments
                          INNER JOIN employees USING (department_id)
                   GROUP BY department_id);

-- 2 c
SELECT department_id, department_name, Count(*)
FROM departments
       INNER JOIN employees USING (department_id)
GROUP BY department_id, department_name
HAVING Count(*) = (SELECT MIN(COUNT(employee_id))
                   FROM departments
                          INNER JOIN employees USING (department_id)
                   GROUP BY department_id);

-- 3.1 Assumption: The exercise asks for actors who played in multiple films
SELECT first_name, last_name, actor_id
FROM actor
       INNER JOIN film_actor USING (actor_id)
GROUP BY first_name, last_name, actor_id
HAVING (COUNT(*) > 1);

-- 3.2
SELECT title
FROM film
WHERE film_id NOT IN (SELECT film_id FROM inventory);

-- 3.3
SELECT r.staff_id, s.store_id, COUNT(*), SUM(amount)
FROM rental r
       INNER JOIN payment USING (rental_id)
       INNER JOIN staff s ON (s.staff_id = r.staff_id)
GROUP BY r.staff_id, s.store_id;

-- 3.4
SELECT first_name, last_name, COUNT(*)
FROM customer
       INNER JOIN rental USING (customer_id)
GROUP BY first_name, last_name
ORDER BY COUNT(*) DESC;

-- 3.5
SELECT customer_id, last_name, store_id, SUM(amount)
FROM payment
       INNER JOIN customer c1 USING (customer_id)
GROUP BY customer_id, last_name, store_id
HAVING SUM(amount) >= ALL (SELECT SUM(amount)
                           FROM payment
                                  INNER JOIN customer c2 USING (customer_id)
                           WHERE c1.store_id = c2.store_id
                           GROUP BY customer_id, last_name, store_id);

-- 3.6
SELECT first_name, last_name, COUNT(*) AS number_of_horror_rental
FROM customer
       INNER JOIN rental USING (customer_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name = 'Horror'
GROUP BY first_name, last_name
HAVING COUNT(*) >= 4
ORDER BY COUNT(*) DESC;

-- 3.7
SELECT actor_id, last_name
FROM actor
       INNER JOIN film_actor USING (actor_id)
GROUP BY actor_id, last_name
ORDER BY COUNT(*) DESC
FETCH FIRST 10 ROWS ONLY;

-- 3.8
SELECT name, category_id, COUNT(*)
FROM category
       INNER JOIN film_category USING (category_id)
       INNER JOIN film USING (film_id)
       INNER JOIN inventory USING (film_id)
GROUP BY category_id, name
ORDER BY COUNT(*) ASC
FETCH FIRST 5 ROWS ONLY;

-- 3.9
SELECT film_id, title, length, category_id
FROM film
       INNER JOIN film_category USING (film_id)
WHERE (category_id, length) IN (SELECT category_id, MAX(length)
                                FROM film
                                       INNER JOIN film_category USING (film_id)
                                GROUP BY category_id)
ORDER BY length ASC
FETCH FIRST 1 ROWS WITH TIES;

-- 4.1
SELECT rating, name, COUNT(*) AS "Nr. Of Films", SUM(length) AS "Total Length (All FIlms)"
FROM film
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN('Comedy', 'Music')
GROUP BY ROLLUP (rating, name);

-- 4.2
SELECT rating, name, COUNT(*) AS "Nr. Of Films", SUM(length) AS "Total Length (All FIlms)"
FROM film
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN('Comedy', 'Music')
GROUP BY GROUPING SETS ( (rating),
                       (name, rating),
                       ());

-- 4.3
SELECT rating, name, COUNT(*) AS "Nr. Of Films", SUM(length) AS "Total Length (All Films)"
FROM film
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN('Comedy', 'Music')
GROUP BY rating, name
UNION ALL
SELECT rating, NULL AS name, COUNT(*) AS "Nr. Of Films", SUM(length) AS "Total Length (All Films)"
FROM film
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN('Comedy', 'Music')
GROUP BY rating, NULL
UNION ALL
SELECT NULL AS rating, NULL AS name, COUNT(*) AS "Nr. Of Films", SUM(length) AS "Total Length (All Films)"
FROM film
       INNER JOIN film_category USING (film_id)
       INNER JOIN category USING (category_id)
WHERE name IN('Comedy', 'Music');

-- 5.1
SELECT manager_staff_id, store_id, SUM(amount)
FROM store s
       INNER JOIN staff USING (store_id)
       INNER JOIN payment USING (staff_id)
GROUP BY CUBE (manager_staff_id, store_id);

-- 5.2
SELECT manager_staff_id, store_id, SUM(amount),
       GROUPING(manager_staff_id) grp_msi,
       GROUPING(store_id) grp_sid
FROM store s
       INNER JOIN staff USING (store_id)
       INNER JOIN payment USING (staff_id)
GROUP BY CUBE (manager_staff_id, store_id);

-- 5.3
SELECT manager_staff_id, store_id, staff_id, SUM(amount)
FROM store s
       INNER JOIN staff USING (store_id)
       INNER JOIN payment USING (staff_id)
GROUP BY GROUPING SETS ((manager_staff_id, store_id, staff_id),
                       (manager_staff_id, store_id),
                       (store_id, staff_id));

-- 5.4
SELECT country, EXTRACT(YEAR FROM payment_date), r.staff_id, sum(amount), count(*)
FROM payment
       INNER JOIN rental r USING (rental_id)
       INNER JOIN staff s ON r.staff_id = s.staff_id
       INNER JOIN inventory i USING (inventory_id)
       INNER JOIN store sto ON sto.store_id = i.store_id
       INNER JOIN address a ON sto.address_id = a.address_id
       INNER JOIN city USING (city_id)
       INNER JOIN country USING (country_id)
GROUP BY GROUPING SETS ( (country_id, country, EXTRACT(YEAR FROM payment_date)),
                       (r.staff_id, EXTRACT(YEAR FROM payment_date)),
                       ());
