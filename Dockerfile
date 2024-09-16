FROM postgres:latest

ENV POSTGRES_USER=postgres
ENV POSTGRES_PASSWORD=postgres
ENV POSTGRES_DB=testdb

COPY postgresql/init.sql /docker-entrypoint-initdb.d/

EXPOSE 5432