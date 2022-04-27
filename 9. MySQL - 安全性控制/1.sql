-- (1) 创建用户tom和jerry，初始密码均为'123456'；
create user tom identified by "123456";
create user jerry identified by "123456";
-- (2) 授予用户tom查询客户的姓名，邮箱和电话的权限,且tom可转授权限；
grant select (c_mail, c_name, c_phone) on client to tom with grant option;
-- (3) 授予用户jerry修改银行卡余额的权限；
grant update (b_balance) on bank_card to jerry;
-- (4) 收回用户Cindy查询银行卡信息的权限。
revoke select on bank_card from Cindy;