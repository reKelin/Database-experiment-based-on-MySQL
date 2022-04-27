mysqldump -h127.0.0.1 -uroot --databases train > train_bak.sql;
mysqladmin flush-logs