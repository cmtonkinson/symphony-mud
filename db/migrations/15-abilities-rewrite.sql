ALTER TABLE `avatars` DROP `abilities`;

DROP TABLE IF EXISTS `abilities`;
CREATE TABLE IF NOT EXISTS `abilities` (
  `avatar` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `mastery` int(10) unsigned NOT NULL DEFAULT '0',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  KEY `avatar` (`avatar`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
