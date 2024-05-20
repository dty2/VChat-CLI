CREATE TABLE userinfo (
  id INT PRIMARY KEY,
  password INT,
  username TEXT
);
CREATE TABLE friendinfo (
  id INT,
  friendid INT,
  foreign key (id) references userinfo(id)
);
CREATE TABLE messageinfo (
  sid INT, -- sender
  rid INT, -- receiver
  message TEXT,
  foreign key (sid) references userinfo(id)
);
INSERT INTO userinfo VALUES (0001, 123456, 'hunter');
INSERT INTO userinfo VALUES (0002, 123456, 'test1');
INSERT INTO userinfo VALUES (0003, 123456, 'test2');
INSERT INTO friendinfo VALUES (0001, 0002);
INSERT INTO friendinfo VALUES (0001, 0003);
INSERT INTO friendinfo VALUES (0002, 0001);
INSERT INTO friendinfo VALUES (0002, 0003);
INSERT INTO friendinfo VALUES (0003, 0001);
INSERT INTO friendinfo VALUES (0003, 0002);
INSERT INTO messageinfo VALUES (0001, 0002, 'hello, world');
INSERT INTO messageinfo VALUES (0001, 0003, 'hello, world');
INSERT INTO messageinfo VALUES (0002, 0001, 'hello, world');
INSERT INTO messageinfo VALUES (0002, 0003, 'hello, world');
INSERT INTO messageinfo VALUES (0003, 0001, 'hello, world');
INSERT INTO messageinfo VALUES (0003, 0002, 'hello, world');
