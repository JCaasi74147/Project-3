/* code used to query the local postgre sql database */
\COPY (
    SELECT
        a1.actor_id AS actor_id_1,
        a1.name AS actor_name_1,
        a2.actor_id AS actor_id_2,
        a2.name AS actor_name_2,
        m.movie_id,
        m.original_title,
        m.popularity,
        m.vote_average
    FROM
        movie_actor ma1
    JOIN
        movie_actor ma2 ON ma1.movie_id = ma2.movie_id AND ma1.actor_id < ma2.actor_id
    JOIN
        actors a1 ON ma1.actor_id = a1.actor_id
    JOIN
        actors a2 ON ma2.actor_id = a2.actor_id
    JOIN
        movies m ON ma1.movie_id = m.movie_id
    JOIN
        (SELECT actor_id FROM movie_actor GROUP BY actor_id HAVING COUNT(movie_id) > 3) subq1 ON ma1.actor_id = subq1.actor_id
    JOIN
        (SELECT actor_id FROM movie_actor GROUP BY actor_id HAVING COUNT(movie_id) > 3) subq2 ON ma2.actor_id = subq2.actor_id
) TO 'actor-movie-data.csv' WITH CSV HEADER;

