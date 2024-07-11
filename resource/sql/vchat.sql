CREATE TABLE userinfo (
  id INT PRIMARY KEY,
  password INT,
  username TEXT
);
CREATE TABLE friendinfo (
  id INT,
  friendid INT,
  friendname TEXT,
  foreign key (id) references userinfo(id)
);
CREATE TABLE messageinfo (
  sender INT,
  receiver INT,
  message TEXT,
  time BIGINT
);
INSERT INTO userinfo VALUES (1, 123456, 'hunter');
INSERT INTO userinfo VALUES (2, 123456, 'test1');
INSERT INTO userinfo VALUES (3, 123456, 'test2');
INSERT INTO friendinfo VALUES (1, 2, 'test1');
INSERT INTO friendinfo VALUES (1, 3, 'test2');
INSERT INTO friendinfo VALUES (2, 1, 'hunter');
INSERT INTO friendinfo VALUES (2, 3, 'test2');
INSERT INTO friendinfo VALUES (3, 1, 'hunter');
INSERT INTO friendinfo VALUES (3, 2, 'test1');
-- example: year:24 month:5 date:28 time:15:49:11
INSERT INTO messageinfo VALUES (1, 2, 'test msg1', 240528154921);
INSERT INTO messageinfo VALUES (1, 3, 'test msg3', 240528154912);
INSERT INTO messageinfo VALUES (2, 1, 'hello, world', 240528154943);
INSERT INTO messageinfo VALUES (2, 3, 'hello, world', 240528154934);
INSERT INTO messageinfo VALUES (3, 1, '你好', 240528154935);
INSERT INTO messageinfo VALUES (3, 2, 'hello, world', 240528154926);
