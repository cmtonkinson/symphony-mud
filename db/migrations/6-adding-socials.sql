-- $Id: 6-adding-socials.sql 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
--
-- This file is part of the Symphony project <http://symphony.thetonk.com>
-- Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published
-- by the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";

--
-- Database: `symphony`
--

-- --------------------------------------------------------

--
-- Table structure for table `socials`
--

DROP TABLE IF EXISTS `socials`;
CREATE TABLE IF NOT EXISTS `socials` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `creator` int(10) unsigned NOT NULL,
  `modifier` int(10) unsigned NOT NULL,
  `name` varchar(255) NOT NULL,
  `targetNone` tinyint(1) NOT NULL DEFAULT '0',
  `targetSelf` tinyint(1) NOT NULL DEFAULT '0',
  `targetVictim` tinyint(1) NOT NULL DEFAULT '0',
  `noneActor` varchar(255) NOT NULL DEFAULT 'You do something.',
  `noneRoom` varchar(255) NOT NULL DEFAULT '$c does something.',
  `selfActor` varchar(255) NOT NULL DEFAULT 'You do something to yourself.',
  `selfRoom` varchar(255) NOT NULL DEFAULT '$c does something to $f.',
  `victimActor` varchar(255) NOT NULL DEFAULT 'You do something to $C.',
  `victimVictim` varchar(255) NOT NULL DEFAULT '$c does something to you.',
  `victimRoom` varchar(255) NOT NULL DEFAULT '$c does something to $C.',
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `avatar` (`creator`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

