drop database if exists GFS;
create database GFS character set utf8;

use GFS;

create table DataServer
(
    `DSID`        int unsigned not null primary key,
    `DiskSize`    bigint unsigned comment 'Ӳ�̴�С',
    `RemainSize`  bigint comment 'ʣ��Ӳ�̴�С',
    `Status`      tinyint not null comment 'DS״̬��0������1������',
    `updatetime`  datetime not null
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

create table BlockInfo
(
    `ID`          int unsigned not null AUTO_INCREMENT primary key,
    `Blockindex`  int not null,
    `DSID`        int unsigned not null,
    `FileID`      bigint not null,
    `Status`      tinyint not null comment '�ļ���״̬��0������1ɾ����2ɾ����',
    `updatetime`  timestamp not null default current_timestamp
)ENGINE=InnoDB DEFAULT CHARSET=utf8;

create table FileInfo
(
    `FileID`     bigint unsigned not null AUTO_INCREMENT primary key,
    `Hash`       varchar(32) not null comment '�ļ�MD5ֵ',
    `FileSize`   bigint comment '�ļ���С',
    `BlockNum`   int not null comment '�ļ������',
    `FinishedNum` int not null default 0 comment '���ϴ�����ļ������',
    `UploadTime` Datetime comment '�ļ��ϴ����ʱ��',
    `Status`     tinyint not null comment '�ļ�״̬��0������1ɾ��'
)ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;



