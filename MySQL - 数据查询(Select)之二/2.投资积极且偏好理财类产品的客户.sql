  -- 2) 投资积极且偏好理财类产品的客户
--   请用一条SQL语句实现该查询：

select t1.pro_c_id from
(
  (
    select pro_c_id, count(distinct(pro_pif_id)) as cnt1 from property, finances_product
    where pro_pif_id = p_id and pro_type = 1
    group by pro_c_id
  ) as t1
  join
  ( 
    select pro_c_id, count(distinct(pro_pif_id)) as cnt2 from property, fund
    where pro_pif_id = f_id and pro_type = 3
    group by pro_c_id
  ) as t2
  on t1.pro_c_id = t2.pro_c_id
)
where t1.cnt1 > t2.cnt2





/*  end  of  your code  */