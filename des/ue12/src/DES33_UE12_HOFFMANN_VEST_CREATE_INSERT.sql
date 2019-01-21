DROP TABLE dim_category;
DROP TABLE dim_date;
DROP TABLE dim_language;
DROP TABLE dim_release_year;
DROP TABLE fact_rental;
DROP SEQUENCE identifiers;

----------------------------- GLOBAL PK GENERATOR -----------------------------
CREATE SEQUENCE identifiers
  START WITH 1
  INCREMENT BY 1
  MAXVALUE 10000000000;

----------------------------- DIMENSION RELEASE YEAR -----------------------------
CREATE TABLE dim_release_year AS
SELECT identifiers.nextval AS dim_release_year_id, year
FROM (
       SELECT DISTINCT release_year AS year
       FROM film
     );

ALTER TABLE dim_release_year
  ADD CONSTRAINT dim_release_year_pk
    PRIMARY KEY (dim_release_year_id);

ALTER TABLE dim_release_year
  MODIFY year NUMBER NOT NULL;

----------------------------- DIMENSION CATEGORY -----------------------------
CREATE TABLE dim_category AS
SELECT identifiers.nextval AS dim_category_id,
       name                AS genre_name,
       'no_classification' AS classification,
       category_id         AS original_category_id
FROM (
       SELECT name, category_id
       FROM category
     );

ALTER TABLE dim_category
  ADD CONSTRAINT
    dim_category_pk PRIMARY KEY (dim_category_id);

UPDATE dim_category
SET classification = 'Storyline'
WHERE genre_name IN ('Animation', 'Sci-Fi', 'Sports');

UPDATE dim_category
SET classification = 'Narrative'
WHERE genre_name IN ('Children', 'Comedy', 'Documentary',
                     'Drama', 'Family', 'Foreign', 'Travel');

UPDATE dim_category
SET classification = 'Mood'
WHERE genre_name IN ('Action', 'Horror', 'Music');

UPDATE dim_category
SET classification = 'Others'
WHERE genre_name NOT IN ('Animation', 'Sci-Fi', 'Sports',
                         'Children', 'Comedy', 'Documentary',
                         'Drama', 'Family', 'Foreign',
                         'Travel', 'Action', 'Horror', 'Music');

----------------------------- DIMENSION LANGUAGE -----------------------------
CREATE TABLE dim_language AS
SELECT identifiers.nextval AS dim_language_id,
       name                AS language_name,
       language_id         AS original_language_id
FROM (
       SELECT name, language_id
       FROM language
     );

ALTER TABLE dim_language
  ADD CONSTRAINT
    dim_language_pk PRIMARY KEY (dim_language_id);

----------------------------- DIMENSION DATE -----------------------------
CREATE TABLE dim_date
(
  dim_date_id NUMBER,
  ddate       DATE   NOT NULL,
  year        NUMBER NOT NULL,
  month       NUMBER NOT NULL,
  quarter     NUMBER NOT NULL,
  day         NUMBER NOT NULL
);

ALTER TABLE dim_date
  ADD CONSTRAINT dim_date_pk PRIMARY KEY (dim_date_id);

DECLARE
  mindate DATE;
  maxdate DATE := TRUNC(SYSDATE);
BEGIN
  SELECT MIN(payment_date) INTO mindate FROM payment;

  FOR i IN 0..(maxdate - mindate)
    LOOP

      INSERT INTO dim_date (dim_date_id, ddate, year, month, quarter, day)
      VALUES (TO_CHAR(mindate + NUMTODSINTERVAL(i, 'day'), 'yyyymmdd'),
              mindate + NUMTODSINTERVAL(i, 'day'),
              EXTRACT(YEAR FROM (mindate + NUMTODSINTERVAL(i, 'day'))),
              EXTRACT(MONTH FROM (mindate + NUMTODSINTERVAL(i, 'day'))),
              TO_CHAR(mindate + NUMTODSINTERVAL(i, 'day'), 'Q'),
              TO_CHAR(mindate + NUMTODSINTERVAL(i, 'day'), 'DD'));
    END LOOP;
END;
/

CREATE MATERIALIZED VIEW fact_rental REFRESH COMPLETE
  START WITH (add_months(TRUNC(SYSDATE, 'mm'), 1) + 0) - 1 / 24
  NEXT (add_months(TRUNC(SYSDATE, 'mm'), 2) + 0) - 1 / 24
AS
SELECT CEIL(return_date - rental_date) days,
       amount,
       dim_language.dim_language_id,
       dim_release_year.dim_release_year_id,
       dim_category.dim_category_id,
       dim_date.dim_date_id
FROM rental
       LEFT JOIN payment USING (rental_id)
       INNER JOIN inventory USING (inventory_id)
       INNER JOIN film USING (film_id)
       INNER JOIN film_category USING (film_id)
       INNER JOIN dim_category ON (film_category.category_id = dim_category.original_category_id)
       INNER JOIN dim_language ON (film.language_id = dim_language.original_language_id)
       INNER JOIN dim_release_year ON (film.release_year = dim_release_year.year)
       INNER JOIN dim_date ON (TRUNC(payment_date) = ddate);

BEGIN
  dbms_mview.REFRESH('fact_rental');
END;

SELECT *
FROM fact_rental;