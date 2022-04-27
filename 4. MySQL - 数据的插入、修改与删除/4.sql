use finance1;
-- 请用一条SQL语句删除client表中没有银行卡的客户信息：

delete from client where not exists (select * from bank_card where client.c_id = bank_card.b_c_id);