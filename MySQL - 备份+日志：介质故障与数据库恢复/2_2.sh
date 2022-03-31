mysql -h127.0.0.1 -uroot < train_bak.sql;
mysqlbinlog --no-defaults --disable-log-bin log/binlog.000018 | mysql -uroot;