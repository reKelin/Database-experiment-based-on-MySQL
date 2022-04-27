select
    c_id,
    c_name,
    c_id_card,
    c_phone
from client
where c_mail is null;