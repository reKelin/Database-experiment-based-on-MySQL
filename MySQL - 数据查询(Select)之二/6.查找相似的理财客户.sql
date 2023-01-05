 -- 6) 查找相似的理财客户
--   请用一条SQL语句实现该查询：
select * from
(
    select *,
        rank() over(partition by pac order by common desc,pbc asc) as crank
    from
    ( 
        select distinct t1.pro_c_id as pac,
                t2.pro_c_id as pbc, 
                count(t2.pro_c_id) as common
        from property as t1 
        join
        (
            select distinct pro_c_id, pro_pif_id
            from property
            where pro_type = 1 
        )as t2
        on t2.pro_pif_id = t1.pro_pif_id
        where pro_type = 1 and t2.pro_c_id <> t1.pro_c_id
        group by t1.pro_c_id, t2.pro_c_id
    ) as t3
) as t4
where crank <3

/*  end  of  your code  */