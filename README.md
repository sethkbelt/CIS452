# CIS452
/*
CREATE TABLE Employee (
    employee_name CHAR(20),
    email_id VARCHAR(40) PRIMARY KEY,
    phone_num CHAR(10),
    height NUMERIC(4,3), -- 4 total digits, 3 after decimal
    weight NUMERIC(5,2), -- 5 total digits, 2 after decimal
    age INTEGER,
    zipCode INTEGER,
    stateCode CHAR(2));


INSERT INTO Employee VALUES
    ('John Doe','john@example.com','1234567890','5.8','160.5','30','90210','CA');
  INSERT INTO Employee VALUES
	('Jane Smith','jane@example.com','9876543210', '5.6','130.75', '28','10001','NY');
 INSERT INTO Employee VALUES
	('Bob Johnson','bob@example.com','5555555555','6','180.25', '35','49401','MI');
 INSERT INTO Employee VALUES
     ('Alice Brown','alice@example.com','7777777777', '5.9','140','45','49401','MI');
 INSERT INTO Employee VALUES
	('Charlie Green','charlie@example.com','1111111111','5.1','210.75','55','90210','CA');

SELECT * FROM Employee;

CREATE TABLE Customers (
    customer_name VARCHAR(20) UNIQUE, -- I HAVE TO MAKE THIS UNIQUE TO MAKE IT A FOREIGN KEY
    email_id VARCHAR(40) NOT NULL PRIMARY KEY,
    phone_num CHAR(10) UNIQUE,
    height NUMERIC(4,3), -- 4 total digits, 3 after decimal
    weight NUMERIC(5,2), -- 5 total digits, 2 after decimal
    age INTEGER,
    CHECK (age > 18),
    CHECK (weight >= 50 and weight <= 250),
    CHECK (LENGTH(phone_num) = 10)
    );

INSERT INTO Customers VALUES
('kim','kim@gmail.com', '4443332221', '5.8','153.5','38');
INSERT INTO Customers VALUES
('sam','sam@gmail.com', '9876543210', '5.6','183.5','34');
INSERT INTO Customers VALUES
('tom','tom@gmail.com', '9998887776','5.11','170.5','44');
INSERT INTO Customers VALUES
('john','john@gmail.com','4447771111', '6.1','203.5','22');
SELECT * FROM Customers; 
CREATE TABLE Orders (
    order_id INTEGER PRIMARY KEY,
    name VARCHAR(20),
    order_items VARCHAR(100) DEFAULT 'Cart is Empty',
    amount NUMERIC(8,2) DEFAULT 0, -- 8 total digits, 2 after decimal
    date_time_info VARCHAR(30),
	FOREIGN KEY (name) REFERENCES Customers(customer_name)
    );


INSERT INTO Orders VALUES
('111', 'sam', 'bread, ketchup, potatoes, tomatoes', '15.43', '2021-02-17 14:08:25');
INSERT INTO Orders VALUES
('222', 'tom', 'mixer, tv, ipod, sofa', '3100.8',  '2020-07-26 08:43:51');
INSERT INTO Orders VALUES
('333', 'sam', 'laptop, chair, table', '2054.78', '2020-01-04 11:50:41');
INSERT INTO Orders VALUES
('444', 'sam', 'shirt, hoodie, xbox', '981.93', '2019-03-06 15:09:01');
INSERT INTO Orders VALUES
('666', 'kim', 'ipod, ipad, treadmill', '3500.22', '2019-08-21 21:31:41');
INSERT INTO Orders VALUES
('555', 'kim', 'indoor cycle, watch, iphone', '1500.22', '2020-10-10 06:12:18');
*/
SELECT * FROM Orders;
/*
-- 1. Find the employees who live in Michigan and are over 5 feet tall.
SELECT * from Employee
	WHERE Employee.stateCode = 'MI'
	AND
	Employee.Height > '5';

-- 2.	Display all the orders placed by Sam except the ones placed in 2020. 
SELECT * FROM Orders
	WHERE Orders.Name = 'sam'
    AND Orders.date_time_info NOT LIKE '%2020%';

-- 3.	What did kim ordered in the year 2019. 
SELECT order_items FROM Orders
	WHERE Orders.Name = 'kim'
    AND Orders.date_time_info LIKE '%2019%';

-- 4.	Find order_id having the minimum amount
SELECT order_id FROM Orders
	where amount = 
		(select min(amount) from Orders);
*/