use MyDb;

#请在以下空白处添加适当的SQL代码，实现编程要求
#语句1：删除表orderDetail中的列orderDate
alter table orderDetail drop orderDate;
#语句2：添加列unitPrice
alter table orderDetail add unitPrice decimal(10, 2);
