    -- 4) 	查找相似的理财产品

--   请用一条SQL语句实现该查询：
select pro_pif_id, cc, dense_rank() over(order by cc desc) as prank 
from
(
    select pro_pif_id, count(pro_c_id) as cc #找出每款产品被全体客户持有总人数
    from
    (
        select pro_pif_id #找出持有产品数量最多的3客户持有的产品
        from
        (
            select pro_c_id #找出持有产品数量最多的3客户
            from
            (
                select pro_c_id, cnt, dense_rank() over(order by cnt desc) as rk
                from
                (
                    select pro_c_id, count(pro_pif_id) as cnt 
                    from property
                    where pro_pif_id = 14 and pro_type = 1
                    group by pro_c_id
                ) as t1
            ) as t2
            where rk <= 3
        ) as t3
        natural join property
        where property.pro_type = 1 and property.pro_pif_id <> 14
    ) as t4 
    natural join property
    where property.pro_type = 1
    group by pro_pif_id
    order by pro_pif_id
) as t5


    





/*  end  of  your code  */