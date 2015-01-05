-- $Id: 8-adding-boards.sql 355 2010-05-27 03:37:50Z cmtonkinson@gmail.com $
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
-- Table structure for table `notes`
--

DROP TABLE IF EXISTS `notes`;
CREATE TABLE IF NOT EXISTS `notes` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `board` int(10) unsigned NOT NULL,
  `author` int(10) unsigned NOT NULL COMMENT 'avatarID',
  `subject` varchar(255) NOT NULL,
  `body` text NOT NULL,
  `modified` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8;

