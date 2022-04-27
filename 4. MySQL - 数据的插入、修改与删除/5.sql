use finance1;
-- 请用一条update语句将手机号码为“13686431238”的这位客户的投资资产(理财、保险与基金)的状态置为“冻结”。：

update
    property
set pro_status = "冻结"
where exists (
    select
        *
    from client
    where c_phone = "13686431238"
    and c_id = pro_c_id
);