-- 5) 查询任意两个客户的相同理财产品数
--   请用一条SQL语句实现该查询：

select p1.pro_c_id, p2.pro_c_id, count(*) as total_count
from property as p1 inner join property as p2
on p1.pro_pif_id = p2.pro_pif_id
where p1.pro_type = 1 and p2.pro_type = 1
    and p1.pro_c_id <> p2.pro_c_id
group by p1.pro_c_id, p2.pro_c_id
having count(*) >= 2
order by p1.pro_c_id;





/*  end  of  your code  */